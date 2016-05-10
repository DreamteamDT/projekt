#include "definition.h"

extern void init(Player *);
extern void doRender(Player *man,Bullet b[]);
extern int processEvents(Player *man,Bullet b[]);

extern void clearCartridge(Bullet ammo[]);
extern void Quit();

extern void loadAmmo(Bullet b[]);
//extern void initBullet();
extern SDL_Texture *initBullet();

extern void updateLogic(Player *p, Bullet b[]);

int global = 0;
int main(int argc, char *argv[])
{

  Player player;

  init(&player);

  bullet.texture=initBullet();
  Bullet ammo[20];

  int done = 0;

  //link(ammo);
  //Event loop
  clearCartridge(ammo);

  while(!done)
  {
    done = processEvents(&player,ammo);
    updateLogic(&player,ammo);
    doRender(&player,ammo);
    //don't burn up the CPU
    SDL_Delay(10);
  }
  Quit();

  // Close and destroy the window

  return 0;
}

