#include <D:\mingw_dev_lib\SDL-1.2.15\include\SDL\SDL.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SDL.h>
#include <SDL_net.h>
#include <time.h>

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


struct enemy
{
    int id;
    int x,y;
    SDL_Surface* bitmap;
    int exists;
    SDL_Rect srcRect,dstRect;
};


int main(int argc, char **argv)
{
    int maxPlayers = 10;
    int j,type;
    char tmp[1024];
    int id,enemyid;
    struct enemy enemies[maxPlayers];

    SDL_Init( SDL_INIT_EVERYTHING );
    SDLNet_Init();

    for(j=0; j<maxPlayers; j++) //Börja med 0 spelare
    {
        enemies[j].exists = 0;
    }

    UDPsocket rcvSock;
    UDPsocket sendSock;
    SDLNet_SocketSet socketset=SDLNet_AllocSocketSet(30);
    Uint16 port;
    port=(Uint16) strtol(argv[2],NULL,0);
    IPaddress ip;
    SDLNet_ResolveHost(&ip,argv[1],port);

    if(!(sendSock = SDLNet_UDP_Open(0)))
    {
        printf("Couldnt open socket\n");
        SDLNet_Quit();
        SDL_Quit();
        return 1;
    }
    IPaddress *myaddress = SDLNet_UDP_GetPeerAddress(sendSock, -1);
    if(!myaddress)
    {
        printf("Could not get own port\n");
        exit(2);
    }
    printf("my port: %d\n",myaddress->port);

    UDPpacket *rcvPack = SDLNet_AllocPacket(1024);
    if(!rcvPack)
    {
        printf("Could not allocate receiving packet\n");
        return 1;
    }

    rcvSock = SDLNet_UDP_Open(myaddress->port);
    if(!rcvSock)
    {
        printf("Could not allocate receiving socket\n");
        return 1;
    }
    UDPpacket *p = SDLNet_AllocPacket(1024);
    if(!p)
    {
        printf("could not allocate packet\n");
        return 1;
    }

    p->address.host = ip.host;
    p->address.port = ip.port;
    type = 0;
    sprintf(p->data,"%d",type);
    p->len = 20;
    SDLNet_UDP_Send(sendSock,-1,p);
    int waiting = 1;


    while(!(SDLNet_UDP_Recv(rcvSock,rcvPack)))
    {
    }

    printf("ej recv\n");

    sscanf(rcvPack->data,"%d %d",&type,&id);
    printf("my ID: %d\n",id);

    SDLNet_UDP_AddSocket(socketset,rcvSock);

    int test;
    SDL_Surface* screen = SDL_SetVideoMode( WINDOW_WIDTH, WINDOW_HEIGHT, 0,0);
    SDL_WM_SetCaption( WINDOW_TITLE, 0 );
    SDL_Surface* bitmap = SDL_LoadBMP("bat.bmp");
    SDL_SetColorKey(bitmap, SDL_SRCCOLORKEY, SDL_MapRGB(bitmap->format, 255, 0, 255));

    int change=0;
    int batImageX = 20;
    int batImageY = 58;
    int batWidth = 75;
    int batHeight = 50;

    srand(time(NULL));
    // We change these to make the bat move
    int batX = rand()%576;
    int batY = rand()%437;
    type = 2;
    printf("innan send data\n");
    sprintf(p->data,"%d %d %d %d",type,id,batX,batY); //Skickar data vid connection
    SDLNet_UDP_Send(sendSock,-1,p);
    printf("connected\n");
    int enemyX;
    int enemyY;

    SDL_Event event;
    bool gameRunning = true;

    SDL_Rect batSource;
    batSource.x = batImageX;
    batSource.y = batImageY;
    batSource.w = batWidth;
    batSource.h = batHeight;

    SDL_Rect batDest;
    batDest.x = batX;
    batDest.y = batY;
    batDest.w = batWidth;
    batDest.h = batHeight;

    SDL_EnableKeyRepeat(20,20);

    while (gameRunning)
    {
        while((SDLNet_CheckSockets(socketset,0)>0))
        {

            SDLNet_UDP_Recv(rcvSock,rcvPack);
            printf("%s\n",rcvPack->data);
            sscanf(rcvPack->data,"%d %d %d %d",&type,&enemyid,&enemyX,&enemyY);
            enemies[enemyid].x = enemyX;
            enemies[enemyid].y = enemyY;
            if(!enemies[enemyid].exists) //Om ny fiende
            {
                enemies[enemyid].bitmap = SDL_LoadBMP("bat.bmp");
                SDL_SetColorKey(enemies[enemyid].bitmap,SDL_SRCCOLORKEY,SDL_MapRGB(enemies[enemyid].bitmap->format,255,0,255));
                enemies[enemyid].dstRect.w = 75;
                enemies[enemyid].dstRect.h = 50;
                enemies[enemyid].exists = 1;
                type = 2;
                sprintf(p->data,"%d %d %d %d",type,id,batX,batY);
                SDLNet_UDP_Send(sendSock,-1,p);
            }
            if(type == 3)
            {
                SDL_FreeSurface(enemies[enemyid].bitmap);
                enemies[enemyid].exists = 0;
                SDL_FillRect(screen,&(enemies[enemyid].dstRect),SDL_MapRGB(screen->format,0,0,0));
            }
        }
        // Handle input
        while(SDL_PollEvent(&event) != 0)
        {
            if(event.type == SDL_KEYDOWN)
            {
                switch(event.key.keysym.sym)
                {
                case SDLK_UP:
                    batY -= 1;
                    change = 1;
                    type = 2;
                    if(batY<0)batY=0;
                    break;
                case SDLK_DOWN:
                    batY += 1;
                    change = 1;
                    type = 2;
                    if(batY>436)batY=436;
                    break;
                case SDLK_LEFT:
                    batX -= 1;
                    change = 1;
                    type = 2;
                    if(batX<0)batX=0;
                    break;
                case SDLK_RIGHT:
                    batX += 1;
                    change = 1;
                    type = 2;
                    if(batX>575)batX=575;
                    break;
                case SDLK_1:
                    gameRunning = 0;
                    change = 1;
                    type = 3;
                default:
                    break;
                }

            }

        }

        if(change)  //Skickar data om någon tangent har tryckts
        {
            sprintf(p->data,"%d %d %d %d",type,id,batX,batY);
            SDLNet_UDP_Send(sendSock,-1,p);
            change = 0;
        }

        int i;
        for(i=0; i<maxPlayers; i++)
        {

            if(enemies[i].exists)  //Printar ut alla fiender på skärmen
            {
                SDL_FillRect(screen,&(enemies[i].dstRect),SDL_MapRGB(screen->format,0,0,0));
                enemies[i].dstRect.x = enemies[i].x;
                enemies[i].dstRect.y = enemies[i].y;
                SDL_BlitSurface(enemies[i].bitmap,&batSource,screen,&enemies[i].dstRect);
                SDL_Flip(screen);
            }
        }
        //Printar spelaren på skärmen
        batDest.x = batX;
        batDest.y = batY;

        SDL_FillRect(screen,&batDest, SDL_MapRGB(screen->format, 0, 0, 0));
        SDL_BlitSurface(bitmap,&batSource,screen,&batDest);
        SDL_Flip(screen);
        SDL_Delay(10);
    }

    SDLNet_UDP_Close(rcvSock);
    SDLNet_UDP_Close(sendSock);
    SDLNet_Quit();
    SDL_FreeSurface(bitmap);
    SDL_FreeSurface(screen);

    SDL_Quit();

    return 0;
}
