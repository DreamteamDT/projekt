#include "init.h"
#include <stdio.h>




void initPlayer(Player *player)
{
    int i;
    SDL_DestroyTexture(player->background);
    SDL_DestroyTexture(player->texture);
    SDL_DestroyTexture(player->scoreBackground);
    SDL_DestroyTexture(player->bullet);


    SDL_Surface *image,*background,*scoreBg,*bullet;

    if(LINUX)
    {
        bullet = IMG_Load("kula.png");
        background = IMG_Load("Background6.png");
        if(background==NULL)
        {
            //Laddningen av bakgrunden misslyckades
            printf("Unable to load background image!\n");
        }
    }
    else
    {
        bullet =IMG_Load("kula.PNG");
        background = IMG_Load("Background6.PNG");
        if(background==NULL)
        {
            //Laddningen av bakgrunden misslyckades
            printf("Unable to load background image!\n");
        }
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
            image = IMG_Load("spriteTorg.png");
            Mix_PlayMusic(player->sounds.backstreet,-1);

        }

        else if(player->spritePick == 2)
        {
            image = IMG_Load("spriteRussia.png");
            Mix_PlayMusic(player->sounds.cykablyat,-1);
        }
        else if(player->spritePick == 3)
        {
            image = IMG_Load("spriteMurica.png");
            Mix_PlayMusic(player->sounds.america,-1);

        }
        else
        {
            image = IMG_Load("spriteChina.png");
            Mix_PlayMusic(player->sounds.china,-1);
        }
    }
    else
    {
        if(player->spritePick==1)
        {
            image = IMG_Load("spriteTorg.PNG");
            Mix_PlayMusic(player->sounds.backstreet,-1);

        }

        else if(player->spritePick == 2)
        {
            image = IMG_Load("spriteRussia.PNG");
            Mix_PlayMusic(player->sounds.cykablyat,-1);
        }
        else if(player->spritePick == 3)
        {
            image = IMG_Load("spriteMurica.PNG");
            Mix_PlayMusic(player->sounds.america,-1);
        }
        else
        {
            image = IMG_Load("spriteChina.PNG");
            Mix_PlayMusic(player->sounds.china,-1);
        }

    }

    SDL_Texture *texture,*bgtexture,*scoreBgtext,*bullettxt;
    player->background = SDL_CreateTextureFromSurface(program.renderer,background);
    player->texture = SDL_CreateTextureFromSurface(program.renderer,image);
    player->scoreBackground = SDL_CreateTextureFromSurface(program.renderer,scoreBg);
    player->bullet = SDL_CreateTextureFromSurface(program.renderer,bullet);
    SDL_FreeSurface(bullet);
    SDL_FreeSurface(scoreBg);
    SDL_FreeSurface(background);
    SDL_FreeSurface(image);

    player->frameX = 0;
    player->frameY = 0;
    player->alive = 1;

    for(i=0; i<4; i++)
    {
        player->enemies[i].justDied = 0;
    }
}

void initLedges(Player *player)
{
    int i;
    //int x = 221;
    //int y = 121;
    int x = 361;
    int y = 181;
    for (i = 0; i < 6; i++)
    {
        player->ledges[i].w = 40;
        player->ledges[i].h = 66;
        player->ledges[i].x = x;
        player->ledges[i].y = y;
        /*player->ledges[i].w = 124;
        player->ledges[i].h = 103;
        player->ledges[i].x = x;
        player->ledges[i].y = y;*/
        if (i == 0)
        {
            x = 207;
            y = 266;
        }
        else if (i == 1)
        {
            x = 666;
            y = 225;
        }
        else if (i == 2)
        {
            x = 207;
            y = 478;
        }
        else if (i == 3)
        {
            x = 463;
            y = 436;
        }
        else if (i == 4)
        {
            x = 666;
            y = 476;
        }
    }

    x = 0;
    y = 0;
    player->ledges[6].w = 1024;
    player->ledges[6].h = 100;
    player->ledges[6].x = x;
    player->ledges[6].y = y;

}

void initCd(Player *player)
{
    SDL_Surface *timer;
    if(LINUX)
    {
        timer = IMG_Load("cdTimer.png");

    }
    else
    {
        timer = IMG_Load("cdTimer.PNG");
    }
    SDL_Texture *cdTimer;
    cdTimer = SDL_CreateTextureFromSurface(program.renderer,timer);
    SDL_FreeSurface(timer);
    player->cdTimer = cdTimer;
    player->blinkRect.x = 500;
    player->blinkRect.y = 660;
    player->blinkRect.w = 300;
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
