#include "definition.h"

extern void init(Player *);
extern void doRender(Player *man,Bullet b[]); //, Enemy *enemies
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
    int connected, i;
    Player player;
    //Enemy enemies[10];
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
    //choice = 0;
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
    //init(&player.enemies[1]);
      //  printf("asd\n");
         /*player.enemies[1].srcRect.x = 32;
         player.enemies[1].srcRect.y = 0;
         player.enemies[1].srcRect.w = 32;
         player.enemies[1].srcRect.h = 32;

         player.enemies[1].dstRect.x = 320;
         player.enemies[1].dstRect.y = 340;
         player.enemies[1].dstRect.w = 32;
         player.enemies[1].dstRect.h = 32;

         player.enemies[1].exists = 1;

    //init(&player.enemies[2]);
         player.enemies[2].srcRect.x = 32;
         player.enemies[2].srcRect.y = 0;
         player.enemies[2].srcRect.w = 32;
         player.enemies[2].srcRect.h = 32;

         player.enemies[2].dstRect.x = 360;
         player.enemies[2].dstRect.y = 340;
         player.enemies[2].dstRect.w = 32;
         player.enemies[2].dstRect.h = 32;

         player.enemies[2].exists = 1;*/
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
        //printf("1\n");
        if (choice == 1){
            recv_data(&player,&client);
            printf("client connect\n");
        }
        updateLogic(&player,ammo);
        //for (i = 0; i < 10; i++)
        doRender(&player,ammo); //,&enemies[i]
        //don't burn up the CPU
        SDL_Delay(40);
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

