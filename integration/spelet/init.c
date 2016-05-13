#include "init.h"
#include <stdio.h>

void initPlayer(Player *player)
{

    SDL_Surface *image,*background,*scoreBg;

    background = IMG_Load("bhh.PNG");
    if(!(scoreBg = IMG_Load("scoreBackground.PNG")))
    {
        printf("syntax error\n");
    }
    if(player->spritePick==1)
    {
        image = IMG_Load("USA.PNG");
    }

    else if(player->spritePick == 2)
        {
            image = IMG_Load("spriteRussia.PNG");
        }
    else if(player->spritePick == 3)
        {
            image = IMG_Load("spriteMurica.PNG");
        }
    else
        {
            image = IMG_Load("spriteChina.PNG");
        }

    SDL_Texture *texture,*bgtexture,*scoreBgtext;
    bgtexture = SDL_CreateTextureFromSurface(program.renderer,background);
    texture = SDL_CreateTextureFromSurface(program.renderer,image);
    scoreBgtext = SDL_CreateTextureFromSurface(program.renderer,scoreBg);
    SDL_FreeSurface(scoreBg);
    SDL_FreeSurface(background);
    SDL_FreeSurface(image);
    player->x = 220;
    player->y = 140;
    player->frameX = 0;
    player->frameY = 0;
    player->texture = texture;
    player->background = bgtexture;
    player->scoreBackground = scoreBgtext;
}
void initLedges(Player *player)
{
    SDL_Surface *image = IMG_Load("pelare.PNG");
    SDL_Texture *texture;
    texture = SDL_CreateTextureFromSurface(program.renderer,image);
    SDL_FreeSurface(image);
    int i, lx = 70, ly = 300;
    for (i = 0; i < 3; i++)
    {
        player->ledges[i].w = 60;
        player->ledges[i].h = 60;
        player->ledges[i].x = lx;
        player->ledges[i].y = ly;
        player->ledges[i].texture = texture;
        lx += 120;
        ly -= 80;
    }
}

SDL_Texture *initBullet()
{
    SDL_Surface *image = IMG_Load("bullet.PNG");
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
