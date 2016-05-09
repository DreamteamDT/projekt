#include <stdlib.h>
#ifndef _MSC_VER
#include <unistd.h>
#endif
#include <SDL.h>
#include <SDL_net.h>
#include <string.h>
#include <stdio.h>
#include <inttypes.h>

int uncomplete_string(char tmp[])
{
    int i=0;
    while(tmp[i]!='\0')
    {
        if(tmp[i]=='\n')
            return 0;
        i++;
    }
    return 1;
}

struct player
{
    UDPpacket *p;
    int id;
    int exists;
    TCPsocket tcpsock;
    IPaddress ip;

};

int main(int argc, char **argv)
{
    int maxPlayers = 4;
    int x,y,type,id,next=0,offset;
    IPaddress ip;
    char tmp[1024];
    int curid=0;
    int playernum=0;

    TCPsocket tcpsock;
    UDPsocket rcvSock;
    UDPsocket sendSock;
    Uint32 ipaddr;
    Uint16 port;
    struct player players[maxPlayers];
    SDL_Init(0);
    SDLNet_Init();
    int i,k;

    for(i=0; i<maxPlayers; i++) //initiera allt till 0
    {
        players[i].exists = 0;
    }

    port=(Uint16)strtol(argv[1],NULL,0);

    /* Resolve the argument into an IPaddress type */
    SDLNet_ResolveHost(&ip,NULL,port);

    /* open the server socket */
    if(!(rcvSock = SDLNet_UDP_Open(5000)))
    {
        printf("Couldnt open socket\n");
        SDLNet_Quit();
        SDL_Quit();
        return 1;
    }
    printf("opened first sock!\n");

    UDPpacket *rcvPack = SDLNet_AllocPacket(1024);
    if(!rcvPack)
    {
        printf("Could not allocate receiving packet\n");
        return 1;
    }

    tcpsock=SDLNet_TCP_Open(&ip);
    if(!tcpsock)
    {
        printf("couldnt open tcp socket\n");
        return 1;
    }

    SDLNet_SocketSet udpset=SDLNet_AllocSocketSet(10);
    SDLNet_SocketSet tcpset=SDLNet_AllocSocketSet(10);
    SDLNet_UDP_AddSocket(udpset,rcvSock);
    SDLNet_TCP_AddSocket(tcpset,tcpsock);



    while(1)
    {

        // Lägger till ny klient vid ny connection
        players[next].tcpsock = SDLNet_TCP_Accept(tcpsock);
        //printf("test ");
        if(players[next].tcpsock)
        {
            SDLNet_TCP_AddSocket(tcpset,players[next].tcpsock);
            players[next].exists = 1;
            sprintf(tmp,"%d",next);
            printf("New connection. ID for new player: %d\n",next);
            SDLNet_TCP_Send(players[next].tcpsock,tmp,strlen(tmp)+1);
            players[next].ip = *SDLNet_TCP_GetPeerAddress(players[next].tcpsock);
            printf("Players ip: %d players port: %d\n",players[next].ip.host,players[next].ip.port);

            for(i=0; i<maxPlayers; i++) //Hittar ledig spot för nästa klient
            {
                if(!players[i].exists)
                {
                    next = i;
                    i = maxPlayers;
                }
            }

        }

        //check for incoming data
        while(SDLNet_CheckSockets(udpset,0)>0)
        {
            SDLNet_UDP_Recv(rcvSock,rcvPack);
            sscanf(rcvPack->data,"%d %d",&type,&id);
            printf("%s\n",rcvPack->data);


            if(type == 2)
            {
                players[id].ip.port = rcvPack->address.port;
                for(i=0; i<maxPlayers; i++)
                {
                    if(players[i].exists)
                    {

                        if(i!=id)
                        {
                            rcvPack->address = players[i].ip;
                            SDLNet_UDP_Send(rcvSock,-1,rcvPack);
                        }

                    }
                }
            }

        }
        while(SDLNet_CheckSockets(tcpset,0)>0)
        {
            for(i=0; i<maxPlayers; i++)
            {
                if(players[i].exists)
                    if(SDLNet_SocketReady(players[i].tcpsock))
                    {

                        //	  printf("Inkommande paket\n");
                        offset = 0;
                        do
                        {
                            offset+=SDLNet_TCP_Recv(players[i].tcpsock,tmp+offset,1024);
                        }
                        while(uncomplete_string(tmp));

                        sscanf(tmp,"%d %d",&type,&id);

                        if(type == 3)
                        {
                            for(k=0; k<maxPlayers; k++)
                            {
                                if(players[k].exists)
                                    if(k!=i)
                                    {
                                        if(!SDLNet_TCP_Send(players[k].tcpsock,tmp,strlen(tmp)+1))
                                        {
                                            printf("SDLNet_TCP_Send: %s\n", SDLNet_GetError());
                                            return 1;
                                        }
                                    }

                            }
                            SDLNet_TCP_DelSocket(tcpset,players[id].tcpsock);
                            SDLNet_TCP_Close(players[id].tcpsock);
                            players[id].exists = 0;
                            for(i=0; i<maxPlayers; i++) //Hittar första bästa lediga spot
                            {
                                if(!players[i].exists)
                                {
                                    next = i;
                                    i = 10;
                                }
                            }
                        }
                    }
            }
        }

        SDL_Delay(1);
    }

    SDLNet_UDP_Close(sendSock);
    SDLNet_UDP_Close(rcvSock);
    SDLNet_FreeSocketSet(udpset);
    /* shutdown SDL_net */
    SDLNet_Quit();

    /* shutdown SDL */
    SDL_Quit();

    return(0);
}
