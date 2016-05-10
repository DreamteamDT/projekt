#include "definition.h"

extern void init(Player *);
extern void doRender(Player *man,Bullet b[]);
extern int processEvents(Player *man,Bullet b[],int *moved,int *type);

extern void clearCartridge(Bullet ammo[]);
extern void Quit();
extern void sendPosition(Player *man);

extern void loadAmmo(Bullet b[]);
//extern void initBullet();
extern SDL_Texture *initBullet();
extern void updateLogic(Player *p, Bullet b[]);

extern void send_data(Player *man,Network *client,int type);
extern int networkInit(Network *client,Player *man);

int global = 0;
int main(int argc, char *argv[])
{
    int done = 0;
    int connected;
    Player player;
    Network client;
    int choice;
    int newline;
    int moved = 0;
    int type;
    bullet.texture=initBullet();
    Bullet ammo[20];

    printf("Vill du connecta till servern? 1=JA 0=NEJ: ");
    scanf("%d",&choice);
    scanf("%c",&newline);
    if(choice==1)
    {//*******INIT NETWORK***************
        if(!(networkInit(&client,&player)))
        {
            done = 1;
        }
        connected = 1;
    }//**********************************
    else
        connected = 0;

    init(&player);

    //link(ammo);
    //Event loop
    clearCartridge(ammo);

    while(!done)
    {
        done = processEvents(&player,ammo,&moved,&type);
        if(moved && connected)
        {
           send_data(&player,&client,type);
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

