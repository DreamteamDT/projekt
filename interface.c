//
//  F4.c
//  SDLtesta
//
//  Created by Jonas Wåhslen on 2015-03-31.
//  Copyright (c) 2015 wahslen. All rights reserved.
//

#include <stdio.h>
#include <SDL.h>
#include <SDL_image.h>
#include <stdbool.h>
#define SCREEN_WIDTH 75
#define SCREEN_HEIGHT 30

bool init();
void loadMedia();

SDL_Window* gWindow = NULL;
SDL_Renderer* gRenderer = NULL;
SDL_Texture* mNumber = NULL;
SDL_Texture* mSpells = NULL;
SDL_Texture* mProfile = NULL;
SDL_Texture* mBlink = NULL;
SDL_Texture* mFrame = NULL;
SDL_Texture* mLetter = NULL;
SDL_Texture* mBackground = NULL;

SDL_Rect gSpriteNumber[10];
SDL_Rect gSpriteSpells[20];
SDL_Rect gSpriteProfile[10];
SDL_Rect gSpriteBlink[2];
SDL_Rect gSpriteFrame[2];
SDL_Rect gSpriteLetter[10];
SDL_Rect gSpriteBackground[10];

int main(int argc, char* args[])
{
    SDL_Event e;
    SDL_RendererFlip flip = SDL_FLIP_VERTICAL;
    bool quit = false;
    SDL_Rect possition;
    possition.y = 100;
    possition.x = 100;
    possition.h = 16;
    possition.w = 16;



    if (init()) {
        printf("worked\n");
    }
    loadMedia();


    while (!quit) {
             int i, n=60, m=290, k=170, china=0, usa=1, russia=2, kth=3;
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                quit = true;
            }
        }

                SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
                SDL_RenderClear(gRenderer);

                    //background

                        SDL_Rect background;
                        background.x = 0;
                        background.y = 370 ;
                        background.w = 640   ;
                        background.h = 110 ;

                        SDL_RenderSetViewport( gRenderer, &background );
                        SDL_RenderCopy(gRenderer, mFrame, &gSpriteFrame[0], NULL);

                        //SDL_Rect background;
                        background.x = 0;
                        background.y = 0;
                        background.w = 640 ;
                        background.h = 300 ;

                        SDL_RenderSetViewport( gRenderer, &background );
                        SDL_RenderCopy(gRenderer, mBackground, &gSpriteBackground[0], NULL);



                    //Scoreboard
                        //wins
                        SDL_Rect scoreboard;
                        scoreboard.x = 125;
                        scoreboard.y = 375;
                        scoreboard.w = 75;
                        scoreboard.h = 10;

                        SDL_RenderSetViewport( gRenderer, &scoreboard );
                        SDL_RenderCopy(gRenderer, mLetter, &gSpriteLetter[5], NULL);

                        //score china
                        scoreboard.x = 130;
                        scoreboard.y = 385;
                        scoreboard.w = 32;
                        scoreboard.h = 20;

                        SDL_RenderSetViewport( gRenderer, &scoreboard );
                        SDL_RenderCopy(gRenderer, mNumber, &gSpriteNumber[china], NULL);

                        //score usa
                        scoreboard.x = 130;
                        scoreboard.y = 405;
                        scoreboard.w = 32;
                        scoreboard.h = 20;

                        SDL_RenderSetViewport( gRenderer, &scoreboard );
                        SDL_RenderCopy(gRenderer, mNumber, &gSpriteNumber[usa], NULL);

                        //score russia
                        scoreboard.x = 130;
                        scoreboard.y = 425;
                        scoreboard.w = 32;
                        scoreboard.h = 20;

                        SDL_RenderSetViewport( gRenderer, &scoreboard );
                        SDL_RenderCopy(gRenderer, mNumber, &gSpriteNumber[russia], NULL);

                        //score kth
                        scoreboard.x = 130;
                        scoreboard.y = 445;
                        scoreboard.w = 32;
                        scoreboard.h = 20;

                        SDL_RenderSetViewport( gRenderer, &scoreboard );
                        SDL_RenderCopy(gRenderer, mNumber, &gSpriteNumber[kth], NULL);


                //char profile picture
                        SDL_Rect Spellbar;
                        Spellbar.x = 240;
                        Spellbar.y = 420 ;
                        Spellbar.w = 50   ;
                        Spellbar.h = SCREEN_HEIGHT   ;

                        SDL_RenderSetViewport( gRenderer, &Spellbar );
                        SDL_RenderCopy(gRenderer, mProfile, &gSpriteProfile[1], NULL);

                //char Blink spell icon
                        //SDL_Rect Spellbar;
                        Spellbar.x = 440;
                        Spellbar.y = 420 ;
                        Spellbar.w = 50  ;
                        Spellbar.h = SCREEN_HEIGHT   ;

                        SDL_RenderSetViewport( gRenderer, &Spellbar );
                        SDL_RenderCopy(gRenderer, mBlink, &gSpriteBlink[0], NULL);


                //spelares namn


                        //client namn veiwport
                        SDL_Rect Clinet1;
                        Clinet1.x = n;
                        Clinet1.y = 390;
                        Clinet1.w = SCREEN_WIDTH  ;
                        Clinet1.h = SCREEN_HEIGHT/2  ;

                        SDL_RenderSetViewport( gRenderer, &Clinet1 );
                        SDL_RenderCopy(gRenderer, mLetter, &gSpriteLetter[0], NULL);

                        SDL_Rect Clinet2;
                        Clinet2.x = n;
                        Clinet2.y = 410;
                        Clinet2.w = SCREEN_WIDTH  ;
                        Clinet2.h = SCREEN_HEIGHT/2  ;

                        SDL_RenderSetViewport( gRenderer, &Clinet2 );
                        SDL_RenderCopy(gRenderer, mLetter, &gSpriteLetter[1], NULL);

                        SDL_Rect Clinet3;
                        Clinet3.x = n;//vart i skärmen
                        Clinet3.y = 430;
                        Clinet3.w = SCREEN_WIDTH  ;//hur feta ikonerna ska vara
                        Clinet3.h = SCREEN_HEIGHT/2  ;

                        SDL_RenderSetViewport( gRenderer, &Clinet3 );
                        SDL_RenderCopy(gRenderer, mLetter, &gSpriteLetter[2], NULL);

                        SDL_Rect Clinet4;
                        Clinet4.x = n;
                        Clinet4.y = 450;
                        Clinet4.w = SCREEN_WIDTH ;
                        Clinet4.h = SCREEN_HEIGHT/2 ;

                        SDL_RenderSetViewport( gRenderer, &Clinet4 );
                        SDL_RenderCopy(gRenderer, mLetter, &gSpriteLetter[3], NULL);
                          //gör så att nästa sprite hamnar 32 bitar till höger


                    //spellbar
                    for(i=0;i<3;i++){

                        SDL_Rect Spellbar;
                        Spellbar.x = m;
                        Spellbar.y = 420 ;
                        Spellbar.w = 50   ;
                        Spellbar.h = SCREEN_HEIGHT   ;

                        SDL_RenderSetViewport( gRenderer, &Spellbar );
                        SDL_RenderCopy(gRenderer, mSpells, &gSpriteSpells[i], NULL);

                        m+=50;
                    }


                    SDL_RenderPresent(gRenderer);
                   // n=0;
                    //m=2;

    }
    return 0;
}

void loadMedia(){

    //Number
    SDL_Surface* gNumberSurface = IMG_Load("numbers.PNG");
    mNumber = SDL_CreateTextureFromSurface(gRenderer, gNumberSurface);
    gSpriteNumber[ 0 ].x =   0;
    gSpriteNumber[ 0 ].y =   0;
    gSpriteNumber[ 0 ].w =  32;
    gSpriteNumber[ 0 ].h = 32;

    gSpriteNumber[ 1 ].x =  32;
    gSpriteNumber[ 1 ].y =   0;
    gSpriteNumber[ 1 ].w =  32;
    gSpriteNumber[ 1 ].h = 32;

    gSpriteNumber[ 2 ].x = 64;
    gSpriteNumber[ 2 ].y =   0;
    gSpriteNumber[ 2 ].w =  32;
    gSpriteNumber[ 2 ].h = 32;

    gSpriteNumber[ 3 ].x = 96;
    gSpriteNumber[ 3 ].y =   0;
    gSpriteNumber[ 3 ].w =  32;
    gSpriteNumber[ 3 ].h = 32;

    gSpriteNumber[ 4 ].x = 128;
    gSpriteNumber[ 4 ].y =   0;
    gSpriteNumber[ 4 ].w =  32;
    gSpriteNumber[ 4 ].h = 32;

    gSpriteNumber[ 5 ].x = 160;
    gSpriteNumber[ 5 ].y =   0;
    gSpriteNumber[ 5 ].w =  32;
    gSpriteNumber[ 5 ].h = 32;

    gSpriteNumber[ 6 ].x = 192;
    gSpriteNumber[ 6 ].y =   0;
    gSpriteNumber[ 6 ].w =  32;
    gSpriteNumber[ 6 ].h = 32;

    gSpriteNumber[ 7 ].x = 224;
    gSpriteNumber[ 7 ].y =   0;
    gSpriteNumber[ 7 ].w =  32;
    gSpriteNumber[ 7 ].h = 32;

    gSpriteNumber[ 8 ].x = 256;
    gSpriteNumber[ 8 ].y =   0;
    gSpriteNumber[ 8 ].w =  32;
    gSpriteNumber[ 8 ].h = 32;

    gSpriteNumber[ 9 ].x = 288;
    gSpriteNumber[ 9 ].y =   0;
    gSpriteNumber[ 9 ].w =  32;
    gSpriteNumber[ 9 ].h = 32;

    //Spells
    SDL_Surface* gSpellSurface = IMG_Load("spells.PNG");
    mSpells = SDL_CreateTextureFromSurface(gRenderer, gSpellSurface);

    gSpriteSpells[ 0 ].x = 0;
    gSpriteSpells[ 0 ].y = 0;
    gSpriteSpells[ 0 ].w = 32;
    gSpriteSpells[ 0 ].h = 32;

    gSpriteSpells[ 1 ].x = 32;
    gSpriteSpells[ 1 ].y = 0;
    gSpriteSpells[ 1 ].w = 32;
    gSpriteSpells[ 1 ].h = 32;

    gSpriteSpells[ 2 ].x = 64;
    gSpriteSpells[ 2 ].y = 0;
    gSpriteSpells[ 2 ].w = 32;
    gSpriteSpells[ 2 ].h = 32;

    gSpriteSpells[ 3 ].x = 96;
    gSpriteSpells[ 3 ].y = 0;
    gSpriteSpells[ 3 ].w = 32;
    gSpriteSpells[ 3 ].h = 32;

    gSpriteSpells[ 4 ].x = 128;
    gSpriteSpells[ 4 ].y = 0;
    gSpriteSpells[ 4 ].w = 32;
    gSpriteSpells[ 4 ].h = 32;

    gSpriteSpells[ 5 ].x = 160;
    gSpriteSpells[ 5 ].y = 0;
    gSpriteSpells[ 5 ].w = 32;
    gSpriteSpells[ 5 ].h = 32;

    gSpriteSpells[ 6 ].x = 192;
    gSpriteSpells[ 6 ].y = 0;
    gSpriteSpells[ 6 ].w = 32;
    gSpriteSpells[ 6 ].h = 32;

    gSpriteSpells[ 7 ].x = 224;
    gSpriteSpells[ 7 ].y = 0;
    gSpriteSpells[ 7 ].w = 32;
    gSpriteSpells[ 7 ].h = 32;

    gSpriteSpells[ 8 ].x = 256;
    gSpriteSpells[ 8 ].y = 0;
    gSpriteSpells[ 8 ].w = 32;
    gSpriteSpells[ 8 ].h = 32;

    gSpriteSpells[ 9 ].x = 288;
    gSpriteSpells[ 9 ].y = 0;
    gSpriteSpells[ 9 ].w = 32;
    gSpriteSpells[ 9 ].h = 32;

    gSpriteSpells[ 10 ].x = 320;
    gSpriteSpells[ 10 ].y = 0;
    gSpriteSpells[ 10 ].w = 32;
    gSpriteSpells[ 10 ].h = 32;

    SDL_Surface* gCharPciture = IMG_Load("CharProfile.PNG");
    mProfile = SDL_CreateTextureFromSurface(gRenderer, gCharPciture);

    gSpriteProfile[ 0 ].x = 0;
    gSpriteProfile[ 0 ].y = 0;
    gSpriteProfile[ 0 ].w = 32;
    gSpriteProfile[ 0 ].h = 32;

    gSpriteProfile[ 1 ].x = 32;
    gSpriteProfile[ 1 ].y = 0;
    gSpriteProfile[ 1 ].w = 32;
    gSpriteProfile[ 1 ].h = 32;

    gSpriteProfile[ 1 ].x = 64;
    gSpriteProfile[ 1 ].y = 0;
    gSpriteProfile[ 1 ].w = 32;
    gSpriteProfile[ 1 ].h = 32;


    SDL_Surface* gBlinkSurface = IMG_Load("blinkIcon.PNG");
    mBlink = SDL_CreateTextureFromSurface(gRenderer, gBlinkSurface);

    gSpriteBlink[ 0 ].x = 0;
    gSpriteBlink[ 0 ].y = 0;
    gSpriteBlink[ 0 ].w = 32;
    gSpriteBlink[ 0 ].h = 32;

    gSpriteBlink[ 1 ].x = 32;
    gSpriteBlink[ 1 ].y = 0;
    gSpriteBlink[ 1 ].w = 32;
    gSpriteBlink[ 1 ].h = 32;

    gSpriteBlink[ 2 ].x = 64;
    gSpriteBlink[ 2 ].y = 0;
    gSpriteBlink[ 2 ].w = 32;
    gSpriteBlink[ 2 ].h = 32;

    gSpriteBlink[ 3 ].x = 96;
    gSpriteBlink[ 3 ].y = 0;
    gSpriteBlink[ 3 ].w = 32;
    gSpriteBlink[ 3 ].h = 32;

    gSpriteBlink[ 4 ].x = 128;
    gSpriteBlink[ 4 ].y = 0;
    gSpriteBlink[ 4 ].w = 32;
    gSpriteBlink[ 4 ].h = 32;

    gSpriteBlink[ 5 ].x = 160;
    gSpriteBlink[ 5 ].y = 0;
    gSpriteBlink[ 5 ].w = 32;
    gSpriteBlink[ 5 ].h = 32;

    gSpriteBlink[ 6 ].x = 192;
    gSpriteBlink[ 6 ].y = 0;
    gSpriteBlink[ 6 ].w = 32;
    gSpriteBlink[ 6 ].h = 32;



     SDL_Surface* gFrameSurface = IMG_Load("background.PNG");
    mFrame = SDL_CreateTextureFromSurface(gRenderer, gFrameSurface);

    gSpriteFrame[ 0 ].x = 0;
    gSpriteFrame[ 0 ].y = 0;
    gSpriteFrame[ 0 ].w = 300;
    gSpriteFrame[ 0 ].h = 300;

      SDL_Surface* gBackgroundSurface = IMG_Load("Background4.PNG");
    mBackground = SDL_CreateTextureFromSurface(gRenderer, gBackgroundSurface);

    gSpriteBackground[ 0 ].x = 0;
    gSpriteBackground[ 0 ].y = 0;
    gSpriteBackground[ 0 ].w = 716;
    gSpriteBackground[ 0 ].h = 724;


     SDL_Surface* gLetterSurface = IMG_Load("name.PNG");
    mLetter = SDL_CreateTextureFromSurface(gRenderer, gLetterSurface);

    gSpriteLetter[ 0 ].x = 0;//China
    gSpriteLetter[ 0 ].y = 0;
    gSpriteLetter[ 0 ].w = 100;
    gSpriteLetter[ 0 ].h = 32;

    gSpriteLetter[ 1 ].x = 100;//USA
    gSpriteLetter[ 1 ].y = 0;
    gSpriteLetter[ 1 ].w = 100;
    gSpriteLetter[ 1 ].h = 32;

    gSpriteLetter[ 2 ].x = 200;//Russia
    gSpriteLetter[ 2 ].y = 0;
    gSpriteLetter[ 2 ].w = 100;
    gSpriteLetter[ 2 ].h = 32;

    gSpriteLetter[ 3 ].x = 300;//KTH
    gSpriteLetter[ 3 ].y = 0;
    gSpriteLetter[ 3 ].w = 100;
    gSpriteLetter[ 3 ].h = 32;

    gSpriteLetter[ 4 ].x = 400;//Winner
    gSpriteLetter[ 4 ].y = 0;
    gSpriteLetter[ 4 ].w = 100;
    gSpriteLetter[ 4 ].h = 32;

    gSpriteLetter[ 5 ].x = 500;//Win
    gSpriteLetter[ 5 ].y = 0;
    gSpriteLetter[ 5 ].w = 100;
    gSpriteLetter[ 5 ].h = 32;





}

bool init(){
    bool test = true;
    SDL_Init(SDL_INIT_VIDEO);
    gWindow = SDL_CreateWindow("InterfaceV1", SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED, 640, 480, SDL_WINDOW_SHOWN);
    if(gWindow == NULL){
        printf("Fungerar ej\n");
        test = false;
    }
    gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
    if(gRenderer == NULL){
        printf("Fungerar ej\n");
        test = false;
    }
    return test;

}
