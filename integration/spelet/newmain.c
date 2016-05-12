#include "definition.h"

extern void initPlayer(Player *player);
extern void initLedges(Player *player);
extern void doRender(Player *man,Bullet b[]); //, Enemy *enemies
extern int processEvents(Player *man,Bullet b[],int *moved,int *type);
extern void collisionDetect(Player *man, int *moved);

extern void clearCartridge(Bullet ammo[]);
extern void Quit();
extern void sendPosition(Player *man);

extern void loadAmmo(Bullet b[]);
//extern void infitBullet();
extern SDL_Texture *initBullet();
extern void updateLogic(Player *p, Bullet b[]);

extern void send_data(Player *man,Network *client,int type);
extern int networkInit(Network *client,Player *man,const char *ipaddress);
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

//>>>>>>> 4f04477192c6dc006aca1aae1c51b75987a36da2
    int startMenu = 1,pickCharacter = 0,imageNo,exit = 0,ingame = 0;
    int test = 123;
    const char *tmp = (const char*)malloc(100);
    int q = 0;
    int done = 0;
    int connected, i;
    Player player;
    //Ledge ledges[3];
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
        printf("Ange IP du vill connecta till: ");
        fgets(tmp,100,stdin);
        connected = 1;
    }
    else
        connected = 0;

    //link(ammo);
    //Event loop
    clearCartridge(ammo);

    Menu menu,pick;
    initMenu(&menu);
    initPick(&pick);
    SDLNet_Init();
    int testss = 0;


    while(!exit) //HUVUDMENYN
    {
        displayMenu(menu);
        pickCharacter = handleMenu(&exit);

        while(pickCharacter) //PICK CHARACTER-MENYN
        {
            displayMenu(pick);
            ingame = handlePick(&pickCharacter);
            if(ingame)
            {
                initPlayer(&player);
                initLedges(&player);
                if(connected && !(networkInit(&client,&player,tmp)))
                {
                    exit = 1;
                    ingame = 0;
                    pickCharacter = 0;
                }
                if(connected && exit!=1)
                    send_data(&player,&client,2);
            }
            while(ingame) //INGAME
            {

                done = processEvents(&player,ammo,&moved,&type);
                //for (i = 0; i < 3; i++)
                collisionDetect(&player, &moved);
                if(moved && connected)
                {
                    send_data(&player,&client,type);
                    moved = 0;
                }
                if (connected)
                {
                    recv_data(&player,&client,&done);
                }
                updateLogic(&player,ammo);
                for (i = 0; i < 3; i++)
                    doRender(&player,ammo); //,&enemies[i]
                //don't burn up the CPU
                SDL_Delay(40);
                if(done)
                {
                    pickCharacter = 0;
                    ingame = 0;
                    SDLNet_FreeSocketSet(client.udpset);
                    SDLNet_FreeSocketSet(client.tcpset);
                    SDLNet_UDP_Close(client.udpsock);
                    SDLNet_TCP_Close(client.tcpsock);
                }
            }
            exit = 0;
        }
    }
    free(tmp);
    SDLNet_Quit();
    Quit();
    printf("ASD\n");

    // Close and destroy the window

    return 0;
}

