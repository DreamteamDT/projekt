#include "definition.h"


void initMenu(Menu *menu)
{

    SDL_Init(SDL_INIT_VIDEO);

    program.window = SDL_CreateWindow("Game Window",                     // window title
                                      SDL_WINDOWPOS_UNDEFINED,           // initial x position
                                      SDL_WINDOWPOS_UNDEFINED,           // initial y position
                                      1024,                               // width, in pixels
                                      768,                               // height, in pixels
                                      0                                  // flags
                                     );
    program.renderer = SDL_CreateRenderer(program.window, -1, SDL_RENDERER_ACCELERATED);

    SDL_Surface *image = IMG_Load("MainMenu.bmp");
    SDL_Texture *texture;
    texture = SDL_CreateTextureFromSurface(program.renderer,image);
    SDL_FreeSurface(image);
    menu->rect.x = 0;
    menu->rect.y = 0;
    menu->rect.w = 640;
    menu->rect.h = 480;
    menu->texture = texture;
}



int handleMenu(int *exit)
{
    SDL_Event e;
    int x,y;
    int quit = 0;
    int pickCharacter = 0;
    while(!quit)
    {
        //Hantera eventer som står på kö
        while(SDL_PollEvent(&e)!=0)
        {
            SDL_GetMouseState(&x, &y);
            //printf("x=%d \ny=%d\n", x, y);
            //Stäng fönstret & avsluta SDL om användaren klickar på X-rutan på fönstret
            if(e.type==SDL_QUIT)
            {
                quit=1;
                *exit = 1;
            }
            //Stäng fönstret & avsluta SDL om användaren trycker på ESC-knappen på tangentbordet
            // else if(e.key.keysym.sym==SDLK_ESCAPE)
            //{
            //  quit=1;
            //*exit = 1;
            //}
            //Gå till menyn "Choose Character" om användaren trycker på "Play Game"
            else if(x>141 && x<455 && y>172 && y<256 && e.type==SDL_MOUSEBUTTONDOWN)
            {
                if(e.button.button==SDL_BUTTON_LEFT)
                {
                    quit = 1;
                    *exit = 0;
                    pickCharacter = 1;
                    // chooseCharacter(); //Går till funktionen för "Choose Character"
                }

            }
            //Stäng fönstret & avsluta SDL om användaren trycker på "Exit"
            else if(x>141 && x<455 && y>270 && y<349 && e.type==SDL_MOUSEBUTTONDOWN)
            {
                if(e.button.button==SDL_BUTTON_LEFT)
                {
                    *exit = 1;
                    quit=1;
                }
            }
        }

    }
    return pickCharacter;
}

void displayMenu(Menu menu)
{
    SDL_RenderClear(program.renderer);
    SDL_RenderCopy(program.renderer,menu.texture,&menu.rect,&menu.rect);
    SDL_RenderPresent(program.renderer);
}

void initPick(Menu *pick)
{
    SDL_Surface *image = IMG_Load("ChooseCharacter.bmp");
    SDL_Texture *texture;
    texture = SDL_CreateTextureFromSurface(program.renderer,image);
    SDL_FreeSurface(image);
    pick->rect.x = 0;
    pick->rect.y = 0;
    pick->rect.w = 640;
    pick->rect.h = 480;
    pick->texture = texture;
}

int handlePick(int *pickCharacter,Player *man)
{
    int x, y;
    SDL_Event e;
    int ingame = 0;

    //Medan fönstret är öppet
    //Hantera eventer som står på kö
    while(SDL_PollEvent(&e)!=0)
    {
        //Positionen på musen i x-led och y-led
        SDL_GetMouseState(&x, &y);
        //printf("x=%d \ny=%d\n", x, y);
        //Stäng fönstret om användaren klickar på X-rutan på fönstret
        if(e.type==SDL_QUIT)
        {
            ingame=0;
            *pickCharacter = 0;
        }
        //Stäng fönstret om användaren trycker på ESC-knappen på tangentbordet
        else if(e.key.keysym.sym==SDLK_ESCAPE)
        {
            ingame=0;
            *pickCharacter = 0;

        }
        //Återgå till huvudmenyn om användaren trycker back-pilen genom ett vänsterklick
        else if(x>250 && x<379 && y>391 && y<453 && e.type==SDL_MOUSEBUTTONDOWN)
        {
            if(e.button.button==SDL_BUTTON_LEFT)
            {
                //Laddar upp huvudmenyn på fönstret och programräknaren återgår till main-loopen
                *pickCharacter = 0;
                ingame = 0;
            }
        }
        //Användaren väljer första gubben
        else if(x>82 && x<151 && y>223 && y<310 && e.type==SDL_MOUSEBUTTONDOWN)
        {
            if(e.button.button==SDL_BUTTON_LEFT)
            {
                ingame = 1;
                man->spritePick = 1;
                //character(1);
            }
        }
        //Användaren väljer andra gubben
        else if(x>215 && x<273 && y>223 && y<310 && e.type==SDL_MOUSEBUTTONDOWN)
        {
            if(e.button.button==SDL_BUTTON_LEFT)
            {
                ingame = 1;
                man->spritePick = 2;
                //character(2);
            }
        }
        //Användaren väljer tredje gubben
        else if(x>343 && x<418 && y>223 && y<310 && e.type==SDL_MOUSEBUTTONDOWN)
        {
            if(e.button.button==SDL_BUTTON_LEFT)
            {
                ingame = 1;
                man->spritePick = 3;
                //character(3);
            }
        }
        //Användaren väljer fjärde gubben
        else if(x>482 && x<544 && y>223 && y<310 && e.type==SDL_MOUSEBUTTONDOWN)
        {
            if(e.button.button==SDL_BUTTON_LEFT)
            {
                ingame = 1;
                man->spritePick = 4;
                //character(4);
            }
        }
    }

    return ingame;
}
