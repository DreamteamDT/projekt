#include <stdlib.h>
#ifndef _MSC_VER
#include <unistd.h>
#endif
#include <SDL.h>
#include <SDL_net.h>
#include <string.h>
#include <stdio.h>

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
    TCPsocket socket;
    int id;
    int exists;
};

int main(int argc, char **argv)
{
    int maxPlayers = 4;
    int x,y,type,id,next=0,offset;
    IPaddress ip;
    char tmp[1024];
    int curid=0;
    int playernum=0;

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
    TCPsocket server=SDLNet_TCP_Open(&ip);



    while(1)
    {

        // Försöker acca connection
        players[next].socket = SDLNet_TCP_Accept(server);

        if(players[next].socket)
        {

            SDLNet_TCP_AddSocket(sockets,players[next].socket);
            players[next].exists = 1;
            sprintf(tmp,"%d",next);
            printf("New connection\n");
            SDLNet_TCP_Send(players[next].socket,tmp,strlen(tmp)+1);
            for(i=0; i<maxPlayers; i++) //Hittar första bästa lediga spot
            {
                if(!players[i].exists)
                {
                    next = i;
                    i = 10;
                }
            }

        }

        //check for incoming data
        while(SDLNet_CheckSockets(sockets,0)>0)
        {
            printf("fore ");
            for(i=0; i<maxPlayers; i++)
            {
                if(players[i].exists)
                    if(SDLNet_SocketReady(players[i].socket))
                    {

                        //	  printf("Inkommande paket\n");
                        offset = 0;
                        do
                        {
                            offset+=SDLNet_TCP_Recv(players[i].socket,tmp,1024);
                            printf("strlen: %d ",strlen(tmp));
                        }
                        while(uncomplete_string(tmp));

                        sscanf(tmp,"%d %d",&type,&id);

                        if(type==2)
                        {
                            for(k=0; k<maxPlayers; k++)
                            {
                                if(players[k].exists)
                                    if(k!=i)
                                    {
                                        if(!SDLNet_TCP_Send(players[k].socket,tmp,strlen(tmp)+1))
                                        {
                                            printf("SDLNet_TCP_Send: %s\n", SDLNet_GetError());
                                            return 1;
                                        }
                                    }

                            }
                        }
                        else if(type == 3)
                        {
                            for(k=0; k<maxPlayers; k++)
                            {
                                if(players[k].exists)
                                    if(k!=i)
                                    {
                                        if(!SDLNet_TCP_Send(players[k].socket,tmp,strlen(tmp)+1))
                                        {
                                            printf("SDLNet_TCP_Send: %s\n", SDLNet_GetError());
                                            return 1;
                                        }
                                    }

                            }
                            SDLNet_TCP_DelSocket(sockets,players[id].socket);
                            SDLNet_TCP_Close(players[id].socket);
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

        SDL_Delay(10);
    }

    SDLNet_TCP_Close(server);
    SDLNet_FreeSocketSet(sockets);
    /* shutdown SDL_net */
    SDLNet_Quit();

    /* shutdown SDL */
    SDL_Quit();

    return(0);
}
