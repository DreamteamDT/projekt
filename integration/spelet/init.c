#include "init.h"
#include <stdio.h>

void initPlayer(Player *player)
{

    SDL_Surface *image,*background,*scoreBg,*bullet;

    bullet = IMG_Load("kula.PNG");
    background = IMG_Load("bakgrund5.PNG");
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

    SDL_Texture *texture,*bgtexture,*scoreBgtext,*bullettxt;
    bgtexture = SDL_CreateTextureFromSurface(program.renderer,background);
    texture = SDL_CreateTextureFromSurface(program.renderer,image);
    scoreBgtext = SDL_CreateTextureFromSurface(program.renderer,scoreBg);
    bullettxt = SDL_CreateTextureFromSurface(program.renderer,bullet);
    SDL_FreeSurface(bullet);
    SDL_FreeSurface(scoreBg);
    SDL_FreeSurface(background);
    SDL_FreeSurface(image);
    if(player->spritePick ==1)
    {
        player->x = 106;
        player->y = 74;
    }
    else if(player->spritePick==2)
    {
        player->x = 939;
        player->y = 55;
    }
    else if(player->spritePick==3)
    {
        player->x = 902;
        player->y = 529;
    }
    else
    {
        player->x = 117;
        player->y = 474;
    }
    player->frameX = 0;
    player->frameY = 0;
    player->texture = texture;
    player->background = bgtexture;
    player->scoreBackground = scoreBgtext;
    player->bullet = bullettxt;
}
void initLedges(Player *player)
{
    /*SDL_Surface *image = IMG_Load("pelare.PNG");
    SDL_Texture *texture;
    texture = SDL_CreateTextureFromSurface(program.renderer,image);
    SDL_FreeSurface(image);*/
    int i, lx = 70, ly = 300;
    int x = 222;
    int y = 115;
    for (i = 0; i < 4; i++)
    {
        player->ledges[i].w = 124;
        player->ledges[i].h = 103;
        player->ledges[i].x = x;
        player->ledges[i].y = y;
        if (i == 0)
            x += 456;
        else if (i == 1)
            y += 238;
        else if (i == 2)
            x -= 456;
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
