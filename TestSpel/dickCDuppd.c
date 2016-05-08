#include <stdio.h>
#include <SDL.h>
#include <SDL_image.h>
#include <stdbool.h>
#include <math.h>

bool init();
void loadMedia();

SDL_Window* gWindow = NULL;
SDL_Renderer* gRenderer = NULL;
SDL_Texture* mSpaceman = NULL;
SDL_Texture* bullet = NULL;
SDL_Renderer *renderer = NULL;
SDL_Cursor *cursor = NULL;
SDL_bool error = SDL_TRUE;

SDL_Rect gSpriteClips[8];
SDL_Rect bulletArray[0];

struct Vector2d
{
  float x, y;
};


int main(int argc, char* args[])
{
    SDL_Event e;
    SDL_RendererFlip flip = SDL_FLIP_VERTICAL;
    bool quit = false;
    SDL_Rect position;
    position.y = 100;
    position.x = 100;
    position.h = 32;
    position.w = 32;
    int frame = 6;
//    SDL_Rect place;
//    place.y = 50;
//    place.x = 50;
//    place.h = 32;
//    place.w = 32;

    if (init()) {
        printf("worked\n");
    }
    loadMedia();
    unsigned int spellOne, spellOne_False=0;



    while (!quit) {
        spellOne = SDL_GetTicks();
        while (SDL_PollEvent(&e)) {
//            SDL_RenderCopyEx(gRenderer, bullet, &bulletArray[0],&place , 0, NULL, flip);
            if (e.type == SDL_QUIT) {
                quit = true;
            }
            else if (e.type == SDL_MOUSEBUTTONDOWN)
            {
                //SDL_RenderCopyEx(gRenderer, bullet, &bulletArray[0],&place , 0, NULL, flip);
                int apa,apb;
                //place.x -=10;
                SDL_GetMouseState(&apa, &apb);


                printf("Cursor is at %d x,%d y\n", apb,apa);
            }
            else if(e.type == SDL_KEYUP && e.key.keysym.sym  == SDLK_1){
                    int blinkX,blinkY;


                        if(spellOne > spellOne_False+1000){
                            spellOne_False=spellOne-1;
                        //SDL_Delay(500);

                            SDL_GetMouseState(&blinkX, &blinkY);

                            float v_length = sqrtf(((blinkX-position.x)*(blinkX-position.x))+((blinkY-position.y)*(blinkY-position.y)));

                            struct Vector2d unit_vector;
                            struct Vector2d direction;
    //
                            direction.x = blinkX-position.x;
                            direction.y = blinkY-position.y;
                            unit_vector.x = (direction.x)/ v_length;
                            unit_vector.y = (direction.y)/ v_length;

                            SDL_Delay(150);
                            position.x+=(unit_vector.x*100)-16;
                            position.y+=(unit_vector.y*100)-16;

                        }

            }
            else if( e.type == SDL_KEYDOWN )
            {


                //Select surfaces based on key press
                switch( e.key.keysym.sym )
                {

                    case SDLK_w:

                        position.y -= 2;
                        flip = SDL_FLIP_NONE;
                        if(frame == 4)
                            frame = 5;
                        else
                            frame = 4;
                        break;
                    case SDLK_s:
                        position.y += 2;
                        flip = SDL_FLIP_NONE;
                        if(frame == 1)
                            frame = 2;
                        else
                            frame = 1;
                        break;
                    case SDLK_a:
                        position.x -= 4;
                        flip = SDL_FLIP_HORIZONTAL;
                        if(frame == 6)
                            frame = 7;
                        else
                            frame = 6;
                        break;
                    case SDLK_d:
                        position.x += 2;
                        position.x +=2;

                        flip = SDL_FLIP_NONE;
                        if(frame == 6)
                            frame = 7;
                        else
                            frame = 6;
                        break;
                    default:

                        break;
                }
            }

        }
        SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
        //SDL_RenderClear(gRenderer);
        SDL_RenderCopyEx(gRenderer, mSpaceman, &gSpriteClips[frame],&position , 0, NULL, flip);
        //SDL_RenderCopyEx(gRenderer, bullet, &bulletArray[0],&place , 0, NULL, flip);
        SDL_RenderPresent(gRenderer);
        SDL_RenderClear(gRenderer);
    }

    return 0;
}

void loadMedia(){
    cursor = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_CROSSHAIR); //Skapar crosshair
    SDL_SetCursor(cursor);
    SDL_Surface* gSpacemanSurface = IMG_Load("USA.PNG");
    mSpaceman = SDL_CreateTextureFromSurface(gRenderer, gSpacemanSurface);
    gSpriteClips[ 0 ].x =   0;
    gSpriteClips[ 0 ].y =   0;
    gSpriteClips[ 0 ].w =  32;
    gSpriteClips[ 0 ].h = 32;

    gSpriteClips[ 1 ].x =  32;
    gSpriteClips[ 1 ].y =   0;
    gSpriteClips[ 1 ].w =  32;
    gSpriteClips[ 1 ].h = 32;

    gSpriteClips[ 2 ].x = 64;
    gSpriteClips[ 2 ].y =   0;
    gSpriteClips[ 2 ].w =  32;
    gSpriteClips[ 2 ].h = 32;

    gSpriteClips[ 3 ].x = 96;
    gSpriteClips[ 3 ].y =   0;
    gSpriteClips[ 3 ].w =  32;
    gSpriteClips[ 3 ].h = 32;

    gSpriteClips[ 4 ].x = 128;
    gSpriteClips[ 4 ].y =   0;
    gSpriteClips[ 4 ].w =  32;
    gSpriteClips[ 4 ].h = 32;

    gSpriteClips[ 5 ].x = 160;
    gSpriteClips[ 5 ].y =   0;
    gSpriteClips[ 5 ].w =  32;
    gSpriteClips[ 5 ].h = 32;

    gSpriteClips[ 6 ].x = 192;
    gSpriteClips[ 6 ].y =   0;
    gSpriteClips[ 6 ].w =  32;
    gSpriteClips[ 6 ].h = 32;

    gSpriteClips[ 7 ].x = 224;
    gSpriteClips[ 7 ].y =   0;
    gSpriteClips[ 7 ].w =  32;
    gSpriteClips[ 7 ].h = 32;

    SDL_Surface* bulletSurface = IMG_Load("bullet.PNG");
    bullet = SDL_CreateTextureFromSurface(gRenderer, bulletSurface);

    bulletArray[ 0 ].x =   0;
    bulletArray[ 0 ].y =   0;
    bulletArray[ 0 ].w =  32;
    bulletArray[ 0 ].h = 32;


}

bool init(){
    bool test = true;
    SDL_Init(SDL_INIT_VIDEO);
    gWindow = SDL_CreateWindow("SDL F4", SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED, 640, 480, SDL_WINDOW_SHOWN);
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
