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
    TCPsocket socket;
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

    UDPsocket rcvSock;
    UDPsocket sendSock;
    Uint32 ipaddr;
    Uint16 port;
    struct player players[maxPlayers];
    SDL_Init(0);
    SDLNet_Init();
    SDLNet_SocketSet sockets=SDLNet_AllocSocketSet(10);
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


    SDLNet_UDP_AddSocket(sockets,rcvSock);

    while(1)
    {

        //check for incoming data
        while(SDLNet_CheckSockets(sockets,0)>0)
        {
            SDLNet_UDP_Recv(rcvSock,rcvPack);
            sscanf(rcvPack->data,"%d %d",&type,&id);
            printf("%s\n",rcvPack->data);

            if(type == 0)
            {
                for(i=0; i<maxPlayers; i++)
                {
                    if(!players[i].exists)
                    {
                        players[i].id = i;
                        players[i].p = rcvPack;
                        players[i].exists = 1;
                        players[i].ip = rcvPack->address;
                        printf("rcvpack address: %d \n",rcvPack->address.host);
                        sprintf(rcvPack->data,"%d %d",type,players[i].id);
                        printf("%s\n",players[i].p->data);
                        printf("%d %d\n",players[i].p->address.port,players[i].p->address.host);
                        SDLNet_UDP_Send(rcvSock,-1,players[i].p);
                        i = maxPlayers;
                        printf("Ny spelare tillagd!\n");
                    }
                }
            }

            if(type == 2)
            {
                for(i=0; i<maxPlayers; i++)
                {
                    if(players[i].exists)
                    {

                        if(i!=id)
                        {
                            printf("id: %d\n",id);
                            printf("i: %d\n",i);
                            rcvPack->address = players[i].ip;
                            SDLNet_UDP_Send(rcvSock,-1,rcvPack);
                            printf("skickat!\n");
                        }

                    }
                }
            }

            if(type == 3)
            {

            }


        }

        SDL_Delay(1);
    }

    SDLNet_UDP_Close(sendSock);
    SDLNet_UDP_Close(rcvSock);
    SDLNet_FreeSocketSet(sockets);
    /* shutdown SDL_net */
    SDLNet_Quit();

    /* shutdown SDL */
    SDL_Quit();

    return(0);
}
