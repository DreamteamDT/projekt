#ifdef __linux__
#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_net.h>
#include <unistd.h>
#elif _WIN32
#include <stdio.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_net.h>
#endif // _WIN32
#include <inttypes.h>
#include <string.h>
#include <stdlib.h>

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

typedef struct
{
    UDPpacket *p;
    int id;
    int exists,ingame;
    TCPsocket tcpsock;
    IPaddress ip;
    float x,y;
    int kills,deaths;
    int justDied,lastData;

} Player;

struct Program
{
    SDL_Window *window;
    SDL_Renderer *renderer;
};

void getSpawn(int next,Player *player)
{
    int i;
    if(next == 0)
    {
        player->x = 106;
        player->y = 300;
    }
    else if(next == 1)
    {
        player->x = 939;
        player->y = 300;
    }
    else if(next == 2)
    {
        player->x = 902;
        player->y = 529;
    }
    else
    {
        player->x = 117;
        player->y = 474;
    }
}

int main(int argc, char **argv)
{
    int maxPlayers = 4;
    int x,y,type,id,next=0,offset,max,hitid;
    IPaddress ip;
    char tmp[1024];
    int curid=0;
    int playernum=0;
    SDL_Event event;

    TCPsocket tcpsock;
    UDPsocket rcvSock;

    Uint32 ipaddr;
    Uint16 port;
    Player players[maxPlayers];
    SDL_Init(SDL_INIT_EVERYTHING);
    SDLNet_Init();
    int i,k,len,size;
    int killed,died,scoreUpdate = 0;

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

    SDL_Init(SDL_INIT_VIDEO);

    struct Program program;
    program.window = SDL_CreateWindow("Game Window",                     // window title
                                      SDL_WINDOWPOS_UNDEFINED,           // initial x position
                                      SDL_WINDOWPOS_UNDEFINED,           // initial y position
                                      640,                               // width, in pixels
                                      480,                               // height, in pixels
                                      0                                  // flags
                                     );
    program.renderer = SDL_CreateRenderer(program.window, -1, SDL_RENDERER_ACCELERATED);

    int running = 1;
    while(running)
    {

        // Lägger till ny klient vid ny connection
        players[next].tcpsock = SDLNet_TCP_Accept(tcpsock);
        //printf("test ");
        if(players[next].tcpsock)
        {
            if(playernum<maxPlayers)
            {
                SDLNet_TCP_AddSocket(tcpset,players[next].tcpsock);
                players[next].exists = 1;
                players[next].kills = 0;
                players[next].deaths = 0;
                players[next].justDied = 0;
                players[next].lastData = SDL_GetTicks();
                type = 0;
                getSpawn(next,&players[next]);
                sprintf(tmp,"%d %d %f %f \n",type,next,players[next].x,players[next].y);
                printf("New connection. ID for new player: %d\n",next);
                SDLNet_TCP_Send(players[next].tcpsock,tmp,strlen(tmp)+1);
                players[next].ip = *SDLNet_TCP_GetPeerAddress(players[next].tcpsock);

                for(i=0; i<maxPlayers; i++) //Hittar ledig spot för nästa klient
                {
                    if(!players[i].exists)
                    {
                        next = i;
                        i = maxPlayers;
                    }
                }
                playernum++;
            }
            else
            {
                type = 4;
                sprintf(tmp,"%d %d",type,next);
                printf("Server full, new connection aborted.\n",next);
                SDLNet_TCP_Send(players[next].tcpsock,tmp,strlen(tmp)+1);
                SDLNet_TCP_Close(players[next].tcpsock);
            }

        }

        //check for incoming data
        while(SDLNet_CheckSockets(udpset,0)>0)
        {
            SDLNet_UDP_Recv(rcvSock,rcvPack);
            sscanf(rcvPack->data,"%d %d",&type,&id);

            players[id].lastData = SDL_GetTicks();
            if(type == 2)
            {
                if(!((SDL_GetTicks() - players[id].justDied) < 2000))
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
            else if(type == 7)
            {
                sscanf(rcvPack->data,"%d %d %d %d",&type,&id,&killed,&died);
                players[id].kills++;
                players[killed].deaths++;
                for(k=0; k<maxPlayers; k++)
                {
                    if(players[k].exists && k!=id)
                    {
                        rcvPack->address = players[k].ip;
                        SDLNet_UDP_Send(rcvSock,-1,rcvPack);
                    }
                }
                players[killed].justDied = SDL_GetTicks();
                for(k=0; k<maxPlayers; k++)
                {
                    if(players[k].exists)
                    {
                        for(i=0; i<maxPlayers; i++)
                        {
                            if(players[i].exists)
                            {
                                type = 10;
                                rcvPack->address = players[i].ip;
                                sprintf(rcvPack->data,"%d %d %d %d",type,k,players[k].kills,players[k].deaths);
                                SDLNet_UDP_Send(rcvSock,-1,rcvPack);
                            }
                        }
                    }
                }

            }
            else if(type == 8)
            {
                for(k=0; k<maxPlayers; k++)
                {
                    if(players[k].exists)
                        if(k!=id)
                        {
                            rcvPack->address = players[k].ip;
                            SDLNet_UDP_Send(rcvSock,-1,rcvPack);
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
                        max = 0;
                        do
                        {
                            if(!(offset+=SDLNet_TCP_Recv(players[i].tcpsock,tmp+offset,1024)))
                            {
                                printf("TCP Recv failed: %s\n",SDL_GetError());
                                return 1;
                            }
                            printf("tar emot data\n");
                            max++;
                        }
                        while(uncomplete_string(tmp) && max<20);
                        sscanf(tmp,"%d %d",&type,&id);
                        printf("type: %d\n",type);
                        players[id].lastData = SDL_GetTicks();
                        if(max>=20)
                        {
                            type = 3;
                            sprintf(tmp,"%d %d \n",type,i);
                        }
                        if(type == 3)
                        {
                            for(k=0; k<maxPlayers; k++)
                            {
                                if(players[k].exists)
                                    if(k!=i)
                                    {
                                        size=0;
                                        len=strlen(tmp)+1;
                                        while(size<len)
                                        {
                                            if(!(size+=SDLNet_TCP_Send(players[k].tcpsock,tmp+size,len-size)))
                                            {
                                                printf("TCP Send failed: %s\n",SDL_GetError());
                                                return 1;
                                            }
                                        }
                                    }

                            }
                            SDLNet_TCP_DelSocket(tcpset,players[i].tcpsock);
                            SDLNet_TCP_Close(players[i].tcpsock);
                            players[i].exists = 0;
                            playernum--;
                            printf("Successfully disconnected player %d.\n",id);
                            for(i=0; i<maxPlayers; i++) //Hittar första bästa lediga spot
                            {
                                if(!players[i].exists)
                                {
                                    next = i;
                                    i = 10;
                                }
                            }
                        }
                        else if(type == 11)
                        {
                            for(k=0; k<maxPlayers; k++)
                            {
                                if(players[k].exists)
                                    if(k!=i)
                                    {
                                        size=0;
                                        len=strlen(tmp)+1;
                                        while(size<len)
                                        {
                                            if(!(size+=SDLNet_TCP_Send(players[k].tcpsock,tmp+size,len-size)))
                                            {
                                                printf("TCP Send failed: %s\n",SDL_GetError());
                                                return 1;
                                            }
                                        }
                                    }

                            }
                        }

                    }
            }
        }

        for(i=0; i<maxPlayers; i++)
        {
            if(players[i].exists)
            {
                if((SDL_GetTicks()-players[i].lastData > 60000)) /** KICK PLAYER IF AFK FOR >60 SECONDS **/
                {
                    type = 3;
                    sprintf(tmp,"%d %d \n",type,i);
                    for(k=0; k<maxPlayers; k++)
                    {
                        if(players[k].exists)
                            {
                                size=0;
                                len=strlen(tmp)+1;
                                while(size<len)
                                {
                                    if(!(size+=SDLNet_TCP_Send(players[k].tcpsock,tmp+size,len-size)))
                                    {
                                        printf("TCP Send failed: %s\n",SDL_GetError());
                                        return 1;
                                    }
                                }
                            }

                    }
                    SDLNet_TCP_DelSocket(tcpset,players[i].tcpsock);
                    SDLNet_TCP_Close(players[i].tcpsock);
                    players[i].exists = 0;
                    playernum--;
                    printf("Player %d has been kicked for being AFK.\n",i);
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
                        }

                    }

                    running = 0;
                default:
                    break;
                }
            }
        }
        scoreUpdate++;



        SDL_Delay(20);
        if(running == 0)
        {
            for(i=5; i>0; i--)
            {
                printf("Shutting down in %d seconds.\n",i);
                SDL_Delay(1000);
            }
        }
    }

    SDLNet_TCP_Close(tcpsock);
    SDLNet_UDP_Close(rcvSock);
    SDLNet_FreeSocketSet(udpset);
    SDLNet_FreeSocketSet(tcpset);
    // SDL_FreeSurface(screen);
    /* shutdown SDL_net */
    SDLNet_Quit();

    /* shutdown SDL */
    SDL_Quit();

    return(0);
}
