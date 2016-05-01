#include "SDL/SDL.h"
#include "SDL2/SDL_ttf.h"
#include <stdbool.h>

const int WINDOW_WIDTH = 640;
const int WINDOW_HEIGHT = 480;
const char* WINDOW_TITLE = "SDL Start";

int main(int argc, char **argv)
{
   SDL_Init( SDL_INIT_VIDEO );

   TTF_Init();

   SDL_Surface* screen = SDL_SetVideoMode( WINDOW_WIDTH, WINDOW_HEIGHT, 0,
      SDL_HWSURFACE | SDL_DOUBLEBUF );
   SDL_WM_SetCaption( WINDOW_TITLE, 0 );

   TTF_Font* font = TTF_OpenFont("ARIAL.TTF", 12);

   SDL_Color foregroundColor = { 100, 123, 255 };
   SDL_Color backgroundColor = { 0, 0, 0 };

   SDL_Surface* textSurface = TTF_RenderText_Shaded(font, "'Give war a chance'-- some character from Metal Gear Rising",
      foregroundColor, backgroundColor);

   // Pass zero for width and height to draw the whole surface
   SDL_Rect textLocation = { 100, 100, 0, 0 };

   SDL_Event event;
   bool gameRunning = true;



   SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 0, 0, 0));

   SDL_BlitSurface(textSurface, NULL, screen, &textLocation);

   SDL_Flip(screen);

   SDL_Delay(5000);

   SDL_FreeSurface(textSurface);

   TTF_CloseFont(font);

   TTF_Quit();

   SDL_Quit();

   return 0;
}
