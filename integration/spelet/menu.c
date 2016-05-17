#include "definition.h"


void initMenu(Menu *menu,Player *man)
{

    SDL_Init(SDL_INIT_EVERYTHING);

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

     SDL_Surface *ipImage;
     SDL_Texture *ipText;

    if(LINUX)
    {
    ipImage = IMG_Load("enterIP.png");
    ipText = SDL_CreateTextureFromSurface(program.renderer,ipImage);
    }
    else
    {
    ipImage = IMG_Load("enterip.PNG");
    ipText = SDL_CreateTextureFromSurface(program.renderer,ipImage);
    }


    SDL_FreeSurface(ipImage);
    menu->rect.x = 0;
    menu->rect.y = 0;
    menu->rect.w = 1024;
    menu->rect.h = 768;
    menu->texture = texture;
    man->ipTexture = ipText;

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
            else if(x>226 && x<728 && y>275 && y<410 && e.type==SDL_MOUSEBUTTONDOWN)
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
            else if(x>226 && x<728 && y>432 && y<558 && e.type==SDL_MOUSEBUTTONDOWN)
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
    pick->rect.w = 1024;
    pick->rect.h = 768;
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
        else if(x>400 && x<606 && y>626 && y<725 && e.type==SDL_MOUSEBUTTONDOWN)
        {
            if(e.button.button==SDL_BUTTON_LEFT)
            {
                //Laddar upp huvudmenyn på fönstret och programräknaren återgår till main-loopen
                *pickCharacter = 0;
                ingame = 0;
            }
        }
        //Användaren väljer första gubben
        else if(x>131 && x<242 && y>357 && y<496 && e.type==SDL_MOUSEBUTTONDOWN)
        {
            if(e.button.button==SDL_BUTTON_LEFT)
            {
                ingame = 1;
                man->spritePick = 1;
                //character(1);
            }
        }
        //Användaren väljer andra gubben
        else if(x>344 && x<437 && y>357 && y<496 && e.type==SDL_MOUSEBUTTONDOWN)
        {
            if(e.button.button==SDL_BUTTON_LEFT)
            {
                ingame = 1;
                man->spritePick = 2;
                //character(2);
            }
        }
        //Användaren väljer tredje gubben
        else if(x>549 && x<669 && y>357 && y<496 && e.type==SDL_MOUSEBUTTONDOWN)
        {
            if(e.button.button==SDL_BUTTON_LEFT)
            {
                ingame = 1;
                man->spritePick = 3;
                //character(3);
            }
        }
        //Användaren väljer fjärde gubben
        else if(x>771 && x<870 && y>357 && y<496 && e.type==SDL_MOUSEBUTTONDOWN)
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

void generateScoreboard(Player *man)
{
    int i,j,k,scoreposition=657;
    SDL_Surface* surfaceMessage;
    SDL_Color black = {0, 0, 0};
    char *score = (char*)malloc(100);
    TTF_Font* arial = TTF_OpenFont("arialbd.ttf", 48);

    /** FOR PLAYER **/
    if(man->spritePick == 1)
    {
        sprintf(score,"Fag        %d      %d",man->kills,man->deaths);
    }
    else if(man->spritePick == 2)
    {
        sprintf(score,"Russia     %d      %d",man->kills,man->deaths);
    }
    else if(man->spritePick == 3)
    {
        sprintf(score,"Murica    %d      %d",man->kills,man->deaths);
    }
    else
    {
        sprintf(score,"China      %d      %d",man->kills,man->deaths);
    }
    surfaceMessage = TTF_RenderText_Blended(arial,score,black);
    man->score = SDL_CreateTextureFromSurface(program.renderer,surfaceMessage);
    SDL_Rect rect = {164,scoreposition,250,20};
    man->scoreRect = rect;
    SDL_FreeSurface(surfaceMessage);
    scoreposition = scoreposition+20;

    /** FOR ENEMIES **/
    for(i=0; i<4; i++)
    {
        if(man->enemies[i].exists)
        {
            printf("printing enemy %d\n",i);
            if(man->enemies[i].sprite == 1)
            {
                sprintf(score,"Fag        %d      %d",man->enemies[i].kills,man->enemies[i].deaths);
            }
            else if(man->enemies[i].sprite == 2)
            {
                sprintf(score,"Russia     %d      %d",man->enemies[i].kills,man->enemies[i].deaths);
            }
            else if(man->enemies[i].sprite == 3)
            {
                sprintf(score,"Murica    %d      %d",man->enemies[i].kills,man->enemies[i].deaths);
            }
            else
            {
                sprintf(score,"China      %d      %d",man->enemies[i].kills,man->enemies[i].deaths);
            }

            surfaceMessage = TTF_RenderText_Blended(arial,score,black);
            man->enemies[i].score = SDL_CreateTextureFromSurface(program.renderer,surfaceMessage);
            SDL_Rect rect = {164,scoreposition,250,20};
            man->enemies[i].scoreRect = rect;
            SDL_FreeSurface(surfaceMessage);
            scoreposition = scoreposition+20;
        }
    }
}
