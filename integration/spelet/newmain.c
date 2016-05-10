#include "definition.h"

extern void init(Player *);
extern void doRender(Player *man,Bullet b[]);
extern int processEvents(Player *man,Bullet b[],int moved);

extern void clearCartridge(Bullet ammo[]);
extern void Quit();
extern void sendPosition(Player *man);

extern void loadAmmo(Bullet b[]);
//extern void initBullet();
extern SDL_Texture *initBullet();
extern void updateLogic(Player *p, Bullet b[]);

extern void send_position(Player *man,Network *client);
extern int networkInit(Network *client,Player *man);

int global = 0;
int main(int argc, char *argv[])
{
    int done = 0;
    Player player;
    Network client;
    int choice;
    int newline;
    int moved = 0;
    bullet.texture=initBullet();
    Bullet ammo[20];

    printf("Vill du connecta till servern? 1=JA 0=NEJ");
    scanf("%d",&choice);
    scanf("%c",&newline);
    if(choice==1)
    {//*******INIT NETWORK***************
        if(!(networkInit(&client,&player)))
        {
            done = 1;
        }
    }//**********************************

    init(&player);

    //link(ammo);
    //Event loop
    clearCartridge(ammo);

    while(!done)
    {
        done = processEvents(&player,ammo,&moved);
        if(moved)
        {
           send_position(&player,&client);
           moved = 0;
        }
        updateLogic(&player,ammo);
        doRender(&player,ammo);
        //don't burn up the CPU
        SDL_Delay(10);
    }
    SDLNet_FreeSocketSet(client.udpset);
    SDLNet_FreeSocketSet(client.tcpset);
    SDLNet_UDP_Close(client.udpsock);
    SDLNet_TCP_Close(client.tcpsock);
    SDLNet_Quit();
    Quit();

    // Close and destroy the window

    return 0;
}

