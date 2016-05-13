#ifdef __linux__
#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_net.h>
#elif _WIN32
#include <stdlib.h>
#include <stdio.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_net.h>
#endif // _WIN32


typedef struct
{
    SDL_Texture *texture;
    int x, y, w, h;
} Ledge;

typedef struct
{
  SDL_Window *window;
  SDL_Renderer *renderer;
}Program;

typedef struct
{
   float x, y;
   float dx;
   float directionX,directionY;
   float vector_unitX,vector_unitY;
   float v_length;
   SDL_Texture *texture;
   int active;
}Bullet;

typedef struct Vector2d
{
  float x, y;
}Vector2d;

typedef struct
{
    TCPsocket tcpsock;
    UDPsocket udpsock;
    SDLNet_SocketSet tcpset,udpset;
    UDPpacket *sendpack,*rcvpack;
    int type;

}Network;

typedef struct
{
    int x,y;
    int frameX,frameY;
    SDL_Texture *texture;
    int exists;
    SDL_Rect srcRect,dstRect;
    Bullet bullet[20];
}Enemy;

typedef struct
{
   Enemy enemies[10];
   Ledge ledges[3];
   int x, y;
   int frameX,frameY;
   int x1, y1;
   SDL_Texture *texture,*background;
   int thinkTime;
   int shot;
   int id;
   int spritePick;
}Player;

typedef struct
{
    SDL_Texture *texture;
    SDL_Rect rect;
}Menu;

Bullet bullet;
Program program;

