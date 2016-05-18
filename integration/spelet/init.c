#include "definition.h"
#include <stdio.h>


void initPlayer(Player *player)
{
    SDL_DestroyTexture(player->scoreHead);
    SDL_DestroyTexture(player->background);
    SDL_DestroyTexture(player->texture);
    SDL_DestroyTexture(player->scoreBackground);
    SDL_DestroyTexture(player->bullet);

    int i;
    SDL_Surface* surfaceMessage;
    TTF_Font* arial = TTF_OpenFont("arialbd.ttf", 48);
    SDL_Color black = {0, 0, 0};
    surfaceMessage = TTF_RenderText_Blended(arial, "Player      Kills       Deaths", black);
    player->scoreHead = SDL_CreateTextureFromSurface(program.renderer,surfaceMessage);
    SDL_FreeSurface(surfaceMessage);

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
    int k;

    for(i=0; i<4; i++)
    {
        player->enemies[i].justDied = 0;
    }
    for(i=0;i<4;i++)
    {
        for(k=0;k<20;k++)
        {
            if(player->enemies[i].bullet[k].active)
                player->enemies[i].bullet[k].active = 0;
        }
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

void initSounds(Player *man)
{
    man->sounds.bulletShot = Mix_LoadWAV("bulletPop.WAV");
}


void Quit()
{
    SDL_DestroyWindow(program.window);
    SDL_DestroyRenderer(program.renderer);

    // Clean up
    SDL_Quit();

}
