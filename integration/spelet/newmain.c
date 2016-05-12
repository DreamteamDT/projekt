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
extern void recv_data(Player *player,Network *client,int *done);

extern void displayMenu(Menu menu);
extern int handleMenu(int *exit);
extern void initMenu(Menu *menu);
extern void initPick(Menu *pick);
extern int handlePick(int *pickCharacter);

int global = 0;
int main(int argc, char *argv[])
{
//<<<<<<< HEAD
//=======
    int startMenu = 1,pickCharacter = 0,imageNo,exit = 0,ingame = 0;
    int test = 123;
    int q = 0;
//>>>>>>> 7da9c63775333773a13af11a9458512471063795
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

    for(i=0; i<10; i++)
    {
        player.enemies[i].exists = 0;
    }

    if(choice==1)
    {
        //*******INIT NETWORK***************
        if(!(networkInit(&client,&player)))
        {
            done = 1;
        }
        connected = 1;
    }//**********************************
    else
        connected = 0;


    if(connected)
    {
        send_data(&player,&client,2);
    }

    //link(ammo);
    //Event loop
    clearCartridge(ammo);

    Menu menu,pick;
    initMenu(&menu);
    initPick(&pick);

    while(!exit)
    {
        displayMenu(menu);
        pickCharacter = handleMenu(&exit);
        while(pickCharacter)
        {
            displayMenu(pick);
            ingame = handlePick(&pickCharacter);
            if(ingame)
            {
                init(&player);
            }
            while(ingame)
            {

                done = processEvents(&player,ammo,&moved,&type);
                if(moved && connected)
                {
                    send_data(&player,&client,type);
                    moved = 0;
                }
                if (choice == 1)
                {
                    recv_data(&player,&client,&done);
                    //  printf("client connect\n");
                }
                updateLogic(&player,ammo);
                //for (i = 0; i < 10; i++)
                doRender(&player,ammo); //,&enemies[i]
                //don't burn up the CPU
                SDL_Delay(40);
                if(done)
                {
                    pickCharacter = 0;
                    ingame = 0;
                }

            }
            exit = 0;
        }
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

