#ifdef __linux__
#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#elif _WIN32
#include <stdio.h>
#include <SDL.h>
#include <SDL_image.h>
#endif // _WIN32

typedef struct
{
  int x, y;
  short life;
  char *name;
  int shot;
} Man;

typedef struct
{
  SDL_Window *window;
  SDL_Renderer *renderer;
}Program;

typedef struct
{
   int x, y;
   int frameX,frameY;
   SDL_Texture *texture;
   int thinkTime;
   int shot;
}Player;

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

Bullet bullet;
Program program;

