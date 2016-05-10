#include "init.h"

void init(Player *player)
{
    SDL_Init(SDL_INIT_VIDEO);


    program.window = SDL_CreateWindow("Game Window",                     // window title
                            SDL_WINDOWPOS_UNDEFINED,           // initial x position
                            SDL_WINDOWPOS_UNDEFINED,           // initial y position
                            640,                               // width, in pixels
                            480,                               // height, in pixels
                            0                                  // flags
                            );
  program.renderer = SDL_CreateRenderer(program.window, -1, SDL_RENDERER_ACCELERATED);

    SDL_Surface *image = IMG_Load("USA.png");
    SDL_Texture *texture;
    texture = SDL_CreateTextureFromSurface(program.renderer,image);
    SDL_FreeSurface(image);
    player->x = 220;
    player->y = 140;
    player->frameX = 0;
    player->frameY = 0;
    player->texture = texture;
}
SDL_Texture *initBullet()
{
     SDL_Surface *image = IMG_Load("bullet.png");
     SDL_Texture *texture;
     texture = SDL_CreateTextureFromSurface(program.renderer,image);
     return texture;
}
void Quit()
{
  SDL_DestroyWindow(program.window);
  SDL_DestroyRenderer(program.renderer);

  // Clean up
  SDL_Quit();

}
