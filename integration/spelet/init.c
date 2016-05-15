#include "init.h"
#include <stdio.h>

void initPlayer(Player *player)
{

    SDL_Surface *image,*background,*scoreBg,*bullet;

    if(LINUX)
    {
        bullet = IMG_Load("kula.png");
        background = IMG_Load("bakgrund5.png");
    }
    else
    {
        bullet =IMG_Load("kula.PNG");
        background = IMG_Load("bakgrund5.PNG");
    }
    if(LINUX)
    {
       if(!(scoreBg = IMG_Load("scoreBackground.png")))
       {
            printf("syntax error\n");
       }

    }
    else
    {
       if(!(scoreBg = IMG_Load("scoreBackground.PNG")))
       {
            printf("syntax error\n");
      }
    }
   if(LINUX)
   {
      if(player->spritePick==1)
    {
        image = IMG_Load("USA.png");
    }

    else if(player->spritePick == 2)
    {
        image = IMG_Load("spriteRussia.png");
    }
    else if(player->spritePick == 3)
    {
        image = IMG_Load("spriteMurica.png");
    }
    else
    {
        image = IMG_Load("spriteChina.png");
    }
   }
   else
   {
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
    player->alive = 1;
}

void initLedges(Player *player)
{
    /*SDL_Surface *image = IMG_Load("pelare.PNG");
    SDL_Texture *texture;
    texture = SDL_CreateTextureFromSurface(program.renderer,image);
    SDL_FreeSurface(image);*/
    int i, lx = 70, ly = 300;
    int x = 221;
    int y = 121;
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

void initCd(Player *player)
{
    SDL_Surface *timer;
    timer = IMG_Load("cdTimer.PNG");
    SDL_Texture *cdTimer;
    cdTimer = SDL_CreateTextureFromSurface(program.renderer,timer);
    SDL_FreeSurface(timer);
    player->cdTimer = cdTimer;
    player->blinkRect.x = 100;
    player->blinkRect.y = 600;
    player->blinkRect.w = 150;
    player->blinkRect.h = 30;
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
