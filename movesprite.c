#include "SDL/SDL.h"

#define SCREEN_WIDTH  640
#define SCREEN_HEIGHT 480
#define SPRITE_SIZE    32

int main(int argc, char *argv[])
{
  SDL_Surface *screen, *temp, *sprite;
  SDL_Rect rcSprite;  /*Sprite ockuperar en rektangle*/
  SDL_Event event;
  int colorkey,gameover;
  Uint8 *keystate;


  /*Initialize SDL Vidoe*/

    if(SDL_Init(SDL_INIT_VIDEO)<0)
     {
        printf("Could not initialize SDL: %s\n",SDL_GetError());
        exit(1);
     }
          /*Open a screen and initialize audio*/

       screen = SDL_SetVideoMode(SCREEN_WIDTH,SCREEN_HEIGHT,0,SDL_HWPALETTE|SDL_DOUBLEBUF);

    if(screen==NULL)
      {
         printf("Couldn't set screen mode to %d x %d: %s\n", SCREEN_WIDTH, SCREEN_HEIGHT, SDL_GetError());
         exit(1);
      }
           /*Set the screen title*/
     SDL_WM_SetCaption("THE SPRITE",NULL);

     temp   = SDL_LoadBMP("sprite.bmp");

     sprite = SDL_DisplayFormat(temp);
  /*Skapar en kopia av surface för att förhasta blitting processen till vår
  window surface annars tar blitting extra tid*/
  SDL_FreeSurface(temp);/*rensa surfacen*/

  /* setup sprite colorkey and turn on RLE */
  colorkey = SDL_MapRGB(screen->format, 0, 0, 255);
  SDL_SetColorKey(sprite, SDL_SRCCOLORKEY | SDL_RLEACCEL, colorkey);

  /* set sprite position */
  rcSprite.x = 223;
  rcSprite.y = 25;

  gameover = 0;

  /* Hantera rörelse om tangentbord används*/
  while (!gameover)
  {
    /* söker händelse */
    if (SDL_PollEvent(&event))
     {
      /* händelse hittas */
      switch (event.type) {
        /* close button clicked */
        case SDL_QUIT:
          gameover = 1;
          break;

        /* samla i data från tangentbord*/
        case SDL_KEYDOWN:
          switch (event.key.keysym.sym)
          {
            case SDLK_ESCAPE:
            case SDLK_q:
              gameover = 1;
              break;
          }
          break;
      }
    }

    /* hantera sprite rörelse */
    keystate = SDL_GetKeyState(NULL);
    if (keystate[SDLK_LEFT] ) {
      rcSprite.x -= 1;
    }
    if (keystate[SDLK_RIGHT] ) {
      rcSprite.x += 1;
    }
    if (keystate[SDLK_UP] ) {
      rcSprite.y -= 1;
    }
    if (keystate[SDLK_DOWN] ) {
      rcSprite.y += 1;
    }
    /* I fall att sprite träffar sidorna av fönstret */
    if ( rcSprite.x < 0 ) {
      rcSprite.x = 0;
    }
    else if ( rcSprite.x > SCREEN_WIDTH-SPRITE_SIZE ) {
      rcSprite.x = SCREEN_WIDTH-SPRITE_SIZE;
    }
    if ( rcSprite.y < 0 ) {
      rcSprite.y = 0;
    }
    else if ( rcSprite.y > SCREEN_HEIGHT-SPRITE_SIZE ) {
      rcSprite.y = SCREEN_HEIGHT-SPRITE_SIZE;
    }

    SDL_BlitSurface(sprite, NULL, screen, &rcSprite);
    /*Arg1: surface vi använder, som är spriten
      Arg2: en SDL_Rect som lagra gamla x,y värde av spriten, behövs inte
      Arg3: Fönster Surface
      Arg4: Den nya x,y värde som spriten ska visas sig*/
    /* update vår fönster*/
    SDL_UpdateRect(screen, 0, 0, 0, 0);
    /*Updatera blittret. de nästa 4 argument motsvarar x,y,w,h om alla
    är noll då uppdatera fönstret*/
  }

  /* clean up */
  SDL_FreeSurface(sprite);
  SDL_Quit();
  return 0;
}
