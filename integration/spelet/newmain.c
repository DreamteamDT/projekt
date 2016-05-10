#include "definition.h"

extern void init(Player *);
extern void doRender(Player *man,Bullet b[]);
extern int processEvents(Player *man,Bullet b[]);

extern void clearCartridge(Bullet ammo[]);
extern void Quit();
extern void sendPosition(Player *man);

extern void loadAmmo(Bullet b[]);
//extern void initBullet();
extern SDL_Texture *initBullet();

extern void updateLogic(Player *p, Bullet b[]);
extern void networkInit(Network *client);

int global = 0;
int main(int argc, char *argv[])
{

  Player player;

  init(&player);

  bullet.texture=initBullet();
  Bullet ammo[20];

  int done = 0;

  //*******INIT NETWORK***************
  Network client;
  int moved = 0;
  if(!(networkInit(&client)))
  {
      done = 1;
  }




  //**********************************

  //link(ammo);
  //Event loop
  clearCartridge(ammo);

  while(!done)
  {
    done = processEvents(&player,ammo,&change);
    //if(moved)
   // {
     //   send_position(&player);
     //   moved = 0;
   // }
    updateLogic(&player,ammo);
    doRender(&player,ammo);
    //don't burn up the CPU
    SDL_Delay(10);
  }
    SDLNet_FreeSocketSet(client->udpset);
    SDLNet_FreeSocketSet(client->tcpset);
    SDLNet_UDP_Close(client->udpsock);
    SDLNet_TCP_Close(client->tcpsock);
    SDLNet_Quit();
    Quit();

  // Close and destroy the window

  return 0;
}

