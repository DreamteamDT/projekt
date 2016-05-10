#include <D:\mingw_dev_lib\SDL-1.2.15\include\SDL\SDL.h>
#include <stdlib.h>
#ifndef _MSC_VER
#include <unistd.h>
#endif
#include <SDL.h>
#include <SDL_net.h>
#include <string.h>
#include <stdio.h>
#include <inttypes.h>

const int WINDOW_WIDTH = 640;
const int WINDOW_HEIGHT = 480;
const char* WINDOW_TITLE = "SDL Start";

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
    SDL_Event event;

    TCPsocket tcpsock;
    UDPsocket rcvSock;

    Uint32 ipaddr;
    Uint16 port;
    struct player players[maxPlayers];
    SDL_Init(SDL_INIT_EVERYTHING);
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

    SDL_Surface* screen = SDL_SetVideoMode( WINDOW_WIDTH, WINDOW_HEIGHT, 0,0);
    SDL_WM_SetCaption( WINDOW_TITLE, 0 );

    int running = 1;
    while(running)
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

        while(SDL_PollEvent(&event) != 0)
        {
            if(event.type == SDL_KEYDOWN)
            {
                switch(event.key.keysym.sym)
                {
                case SDLK_1:
                    type = 6;
                    sprintf(tmp,"%d \n",type);
                    for(k=0; k<maxPlayers; k++)
                            {
                                if(players[k].exists)
                                    {
                                        SDLNet_TCP_Send(players[k].tcpsock,tmp,strlen(tmp)+1);

                                        printf("Shutting down in 5 seconds.\n",k);
                                    }

                            }
                    running = 0;
                default:
                    break;
                }

            }

        }

        SDL_Delay(1);
        if(running == 0)
        {
            SDL_Delay(5000);
        }
    }

    SDLNet_TCP_Close(tcpsock);
    SDLNet_UDP_Close(rcvSock);
    SDLNet_FreeSocketSet(udpset);
    SDLNet_FreeSocketSet(tcpset);
    SDL_FreeSurface(screen);
    /* shutdown SDL_net */
    SDLNet_Quit();

    /* shutdown SDL */
    SDL_Quit();

    return(0);
}
