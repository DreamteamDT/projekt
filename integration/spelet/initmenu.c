#include "definition.h"


void initMenu(Menu *menu)
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
