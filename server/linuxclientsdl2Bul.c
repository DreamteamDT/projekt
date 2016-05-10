#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SDL2/SDL.h>

#include <SDL2/SDL_net.h>
#include <SDL2/SDL_image.h>
#include <time.h>

const int WINDOW_WIDTH = 640;
const int WINDOW_HEIGHT = 480;
const char* WINDOW_TITLE = "SDL Start";

#define BAT_IMAGE_X 20
#define BAT_IMAGE_Y 58
#define BAT_WIDTH 75
#define BAT_HEIGHT 50

#define BUL_WIDTH 13
#define BUL_HEIGHT 7

int uncomplete_string(char tmp[]){
  int i=0;
  while(tmp[i]!='\0'){
    if(tmp[i]=='\n')
      return 0;
    i++;
  }
  return 1;
}
TCPsocket sock;
void send_data(int type,int id,int x,int y){
  char tmp[1024];
  sprintf(tmp,"%d %d %d %d \n",type,id,x,y);
  int size=0;
  int len=strlen(tmp);
  while(size<len){

    size+=SDLNet_TCP_Send(sock,tmp+size,len-size);
  }
}

typedef struct
{
  int id;
  int x,y,bul;
  int cT, lT;
  int exists;
  SDL_Rect srcRect, dstRect, bulRect;
} enemy;

typedef struct
{
    // players
    enemy enemies[10];

    SDL_Rect batSource;

    // images
    SDL_Texture *bat;
    SDL_Texture *bullet;

    // renderer
    SDL_Renderer *renderer;
} GameState;

bool colDet(int batX, int batY, SDL_Rect* enemyPos);
bool bulDet(int batX, int batY, SDL_Rect* enemyPos);
void collisionDetect(GameState *game);
void doRender(SDL_Renderer *renderer, GameState *game);
void loadGame(GameState *game);

int main(int argc, char **argv)
{
  SDL_Init( SDL_INIT_EVERYTHING );
  int maxPlayers = 10;
  int j,type,i, myId, w = 0;
  GameState gameState;
  //enemy enemies[maxPlayers];

  for(j=0;j<maxPlayers;j++){ //Börja med 0 spelare
    gameState.enemies[j].exists = 0;
    gameState.enemies[j].bul = 0;
    gameState.enemies[j].lT = 0;
  }
  IPaddress ip;
  char tmp[1024];
  int id,enemyid, eneXzit = 0;
  Uint16 port;
  SDLNet_SocketSet socketset=SDLNet_AllocSocketSet(30);

  SDLNet_Init();
  port=(Uint16) strtol(argv[2],NULL,0);
  SDLNet_ResolveHost(&ip,argv[1],port);
  sock = SDLNet_TCP_Open(&ip);
  if(!sock){
    printf("Couldnt open socket\n");
    return 1;
  }
  SDLNet_TCP_Recv(sock,tmp,1024);
  sscanf(tmp,"%d",&id);
  myId = id;
  printf("my ID: %d\n",id);
  gameState.enemies[id].exists = 1;

  SDLNet_TCP_AddSocket(socketset,sock);
  int test;
  /* SDL 1.2
  SDL_Surface* screen = SDL_SetVideoMode( WINDOW_WIDTH, WINDOW_HEIGHT, 0,0);
  SDL_WM_SetCaption( WINDOW_TITLE, 0 );
  SDL_Surface* bitmap = SDL_LoadBMP("bat.bmp");
  SDL_SetColorKey(bitmap, SDL_SRCCOLORKEY, SDL_MapRGB(bitmap->format, 255, 0, 255));*/

  /*SDL 2 */
  SDL_Window *window;
  SDL_Renderer *renderer;
  window = SDL_CreateWindow("Game Window", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 340, 280, 0);
  renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
  gameState.renderer = renderer;
  loadGame(&gameState);

  int change=0;
  int b[4] = {0,0,0,0};

  int ru = 0;

   srand(time(NULL));
   // We change these to make the bat move
   int batX = rand()%240;
   int batY = rand()%180;
   type = 2;
   printf("innan send data\n");
   send_data(type,id,batX,batY); //Skickar data vid connection
   printf("connected\n");

   int enemyX;
   int enemyY;

   SDL_Event event;
   bool gameRunning = true;


   gameState.batSource.x = BAT_IMAGE_X;
   gameState.batSource.y = BAT_IMAGE_Y;
   gameState.batSource.w = BAT_WIDTH;
   gameState.batSource.h = BAT_HEIGHT;

   gameState.enemies[id].dstRect.x = batX;
   gameState.enemies[id].dstRect.y = batY;
   gameState.enemies[id].dstRect.w = BAT_WIDTH;
   gameState.enemies[id].dstRect.h = BAT_HEIGHT;

   //SDL_EnableKeyRepeat(20,20);
   while (gameRunning)
   {
     //printf("game running\n");
     while(SDLNet_CheckSockets(socketset,0)>0)
     {
       int offset = 0;
       int test;
       do{
	     SDLNet_TCP_Recv(sock,tmp+offset,1024);
       } while(uncomplete_string(tmp));

       sscanf(tmp,"%d %d %d %d",&type,&enemyid,&enemyX,&enemyY);
       gameState.enemies[enemyid].x = enemyX;
       gameState.enemies[enemyid].y = enemyY;

       //Om ny fiende
       if (!gameState.enemies[enemyid].exists)
       {
         gameState.enemies[enemyid].dstRect.w = 75;
         gameState.enemies[enemyid].dstRect.h = 50;
         gameState.enemies[enemyid].exists = 1;
         gameState.enemies[enemyid].id = enemyid;
         send_data(2,id,batX,batY);
         eneXzit = 1;
       }
       if (type == 2)
       {
           gameState.enemies[enemyid].dstRect.x = enemyX;
           gameState.enemies[enemyid].dstRect.y = enemyY;
       }
       if(type == 3)
       {
	     // flyttar spelaren av skärmen
         gameState.enemies[enemyid].dstRect.x = 1000;
         gameState.enemies[enemyid].dstRect.y = 1000;
         doRender(renderer, &gameState);
         gameState.enemies[enemyid].exists = 0;
       }
       if(type == 4)
       {
           gameState.enemies[enemyid].bul = 1;
           gameState.enemies[enemyid].bulRect.x = enemyX + BAT_WIDTH + 4;
           gameState.enemies[enemyid].bulRect.y = enemyY + BAT_HEIGHT/2;
           gameState.enemies[enemyid].bulRect.w = BUL_WIDTH;
           gameState.enemies[enemyid].bulRect.h = BUL_HEIGHT;
           printf("type 4 recv\n");
       }
     }

      // Handle input
     while(SDL_PollEvent(&event) != 0)
     {
       if(event.type == SDL_KEYDOWN)
       {
         switch(event.key.keysym.sym)
         {
         case SDLK_UP:
           batY -= 3;
           change = 1;
           type = 2;
           b[0] = 1;
           if(batY<0)batY=0;
           break;
         case SDLK_DOWN:
           batY += 3;
           change = 1;
           type = 2;
           b[1] = 1;
           if(batY>436)batY=436;
           break;
         case SDLK_LEFT:
           batX -= 3;
           change = 1;
           type = 2;
           b[2] = 1;
           if(batX<0)batX=0;
           break;
         case SDLK_RIGHT:
           batX += 3;
           change = 1;
           type = 2;
           b[3] = 1;
           if(batX>575)batX=575;
           break;
         case SDLK_SPACE:
           change = 1;
           type = 4;
           if(batX>575)batX=575;
           break;
         case SDLK_RETURN:
           gameRunning = 0;
           change = 1;
           type
           = 3;
           break;
         case SDLK_1:
           gameRunning = 0;
           change = 1;
           type = 3;
         default:
           break;
         }
       }
     }

         if(change)
         {
            //collisionDetect(&gameState);
           for(i=0;i<maxPlayers;i++)
           {
             if(colDet(batX, batY, &gameState.enemies[i].dstRect) && i != myId)
             {
               if(b[0]) {
                 batY += 4;
                 change = 0;
                 printf("col\n");
                 printf("%d\n", gameState.enemies[i].id);
               }
               else if(b[1]) {
                 batY -= 4;
                 change = 0;
                 printf("col\n");
                 printf("%d\n", gameState.enemies[i].id);
               }
               else if(b[2]) {
                 batX += 4;
                 change = 0;
                 printf("col\n");
                 printf("%d\n", gameState.enemies[i].id);
               }
               else if(b[3]) {
                 batX -= 4;
                 change = 0;
                 printf("col\n");
                 printf("%d\n", gameState.enemies[i].id);
               }
             }
           }
         }

         // reset collision detection
         for(i=0;i<4;i++)
           b[i] = 0;


         if(change){ //Skickar data om man rört sig
           w = 1;
           send_data(type,id,batX,batY);
           gameState.enemies[id].dstRect.x = batX;
           gameState.enemies[id].dstRect.y = batY;
           if (type == 4)
           {
               gameState.enemies[id].bul = 1;
               gameState.enemies[id].bulRect.x = batX + BAT_WIDTH + 4;
               gameState.enemies[id].bulRect.y = batY + BAT_HEIGHT/2;
               gameState.enemies[id].bulRect.w = BUL_WIDTH;
               gameState.enemies[id].bulRect.h = BUL_HEIGHT;

               printf("type 4\n");
           }
         }
         else
           w = 0;
         change = 0;
         type = 0;
     if (w == 0){
     for (i = 0; i < 10; i++)
     {

         if (gameState.enemies[i].bul)
         {
             //printf("%d\n", gameState.enemies[i].bulRect.x);
             if(bulDet(gameState.enemies[i].bulRect.x, gameState.enemies[i].bulRect.y, &gameState.enemies[id].dstRect) && id != i)
             {
                 // denna client har dött
                 printf("player %d killed player %d \n", i, id);
                 type = 3;
                 send_data(type,id,batX,batY);
                 gameRunning = 0;
                 /*gameState.enemies[i].bul = 0;
                 gameState.enemies[id].dstRect.x = 1000;
                 gameState.enemies[id].dstRect.y = 1000;
                 doRender(renderer, &gameState);
                 gameState.enemies[id].exists = 0;*/
                 printf("type 3\n");
                 type = 0;
             }
         }
     }
     }
     // Render display
     doRender(renderer, &gameState);

     SDL_Delay(90);
     for (i = 0; i < 10; i++)
     {
         if (gameState.enemies[i].bul)
         {
             gameState.enemies[i].cT = SDL_GetTicks();
             if (gameState.enemies[i].cT > gameState.enemies[i].lT + 10)
             {
                //printf("HORSE\n");
                //batDest.x += 4;
                gameState.enemies[i].bulRect.x += 2;
                gameState.enemies[i].lT = gameState.enemies[i].cT;
             }
         }
     }
   }

   // shutdown
   SDLNet_TCP_Close(sock);
   SDLNet_Quit();
   SDL_DestroyTexture(gameState.bat);
   SDL_DestroyTexture(gameState.bullet);

   SDL_DestroyWindow(window);
   SDL_DestroyRenderer(gameState.renderer);

   SDL_Quit();

   return 0;
}


bool colDet(int batX, int batY, SDL_Rect* enemyPos)
{

    if(batY>=(enemyPos->y + BAT_HEIGHT))
      return 0;
    if(batX>=(enemyPos->x + BAT_WIDTH))
      return 0;
    if((batY + BAT_HEIGHT) <=enemyPos->y)
      return 0;
    if((batX + BAT_WIDTH) <=enemyPos->x)
      return 0;
    return 1;
}

bool bulDet(int bulX, int bulY, SDL_Rect* enemyPos)
{

    if(bulY>=(enemyPos->y + BAT_HEIGHT))
      return 0;
    if(bulX>=(enemyPos->x + BAT_WIDTH))
      return 0;
    if((bulY + BUL_HEIGHT) <=enemyPos->y)
      return 0;
    if((bulX + BUL_WIDTH) <=enemyPos->x)
      return 0;
    return 1;
}

void doRender(SDL_Renderer *renderer, GameState *game)
{
    int i;
    // set the drawing color to blue
    SDL_SetRenderDrawColor(renderer, 255, 0, 255, 255);

    // Clear the screen (to blue)
    SDL_RenderClear(renderer);

    // set the drawing color to white
    SDL_SetRenderDrawColor(renderer, 255, 0, 255, 255);

    for (i = 0; i < 10; i++)
    {
        if (game->enemies[i].exists)
        {
            SDL_RenderCopyEx(renderer,game->bat, &game->batSource, &game->enemies[i].dstRect, 0, NULL, 0);
            if (game->enemies[i].bul)
                SDL_RenderCopyEx(renderer,game->bullet, NULL, &game->enemies[i].bulRect, 0, NULL, 0);
        }
    }

    // We are done drawing, "present" or show to the screen what we've drawn
    SDL_RenderPresent(renderer);
}


void loadGame(GameState *game)
{
    SDL_Surface *asdsurface = NULL;

    // load images and create rendering textures from them
    asdsurface = IMG_Load("bat.bmp");

    if (asdsurface == NULL)
    {
        printf("Cannot find bat.bmp!\n\n");
        SDL_Quit();
        exit (1);
    }

    game->bat = SDL_CreateTextureFromSurface(game->renderer, asdsurface);
    SDL_FreeSurface(asdsurface);


    asdsurface = IMG_Load("bullet.png");
    if (asdsurface == NULL)
    {
        printf("Cannot find bullet.png!\n");
        SDL_Quit();
        exit(1);
    }
    game->bullet = SDL_CreateTextureFromSurface(game->renderer, asdsurface);
    SDL_FreeSurface(asdsurface);
}
