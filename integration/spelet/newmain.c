#include "definition.h"

extern void init(Player *, Ledge *ledges);
extern void doRender(Player *man,Bullet b[], Ledge *ledges); //, Enemy *enemies
extern int processEvents(Player *man,Bullet b[],int *moved,int *type);
extern void collisionDetect(Player *man, int *moved, Ledge *ledges);

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
    Ledge ledges[3];
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
//printf("init\n");

//<<<<<<< HEAD

//=======
//>>>>>>> 4fd6c0fd942b4c795ba0bcb258b485843f18957f

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
//>>>>>>> 4fd6c0fd942b4c795ba0bcb258b485843f18957f
        {
            printf("ASD\n");
            displayMenu(pick);
            ingame = handlePick(&pickCharacter);
            if(ingame)
            {
                int s;
                for (s=0; s < 3; s++)
                {
                    init(&player, &ledges[s]);
                    printf("%d\n", ledges[s].x);
                }
            }
            while(ingame)
            {

                done = processEvents(&player,ammo,&moved,&type);
                for (i = 0; i < 3; i++)
                    collisionDetect(&player, &moved, &ledges[i]);
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
                for (i = 0; i < 3; i++)
                    doRender(&player,ammo, &ledges[i]); //,&enemies[i]
                //don't burn up the CPU
                SDL_Delay(40);
                if(done)
                {
                    pickCharacter = 0;
                    ingame = 0;
                    printf("ASD\n");
                }

            }
            exit = 0;
        }
//<<<<<<< HEAD
//=======
//>>>>>>> 4fd6c0fd942b4c795ba0bcb258b485843f18957f
    }
    SDLNet_FreeSocketSet(client.udpset);
    SDLNet_FreeSocketSet(client.tcpset);
    SDLNet_UDP_Close(client.udpsock);
    SDLNet_TCP_Close(client.tcpsock);
    SDLNet_Quit();
    Quit();
    printf("ASD\n");

    // Close and destroy the window

    return 0;
}

