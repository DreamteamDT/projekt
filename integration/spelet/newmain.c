#include "definition.h"

/** Vi beh�ver tydligen 7inte deklarera funktionerna h�r?? **/
/** Linkern g�r det �t oss **/

extern void initSounds(Player *man);

int global = 0;
int main(int argc, char *argv[])
{

    int pickCharacter = 0,exit = 0,ingame = 0,connected, i,done = 0;
    int choice,newline,moved = 0,type,direct = 0,enterIPmenu;
    unsigned int lastTime,currentTime;
    char *tmp = (char*)malloc(100);
    int frameStart=0,frameEnd=0;
    float lastSent = 0;
    Menu menu,pick;

    Player player = {0};
    Network client;
    player.blinked = 0;
    player.spellReady = 1;
    bullet.texture=initBullet();
    Bullet ammo[20];

    TTF_Init();

    int spawnTimer=4;
    srand(time(NULL));

    connected = 1;

    /** INIT SOUNDS,PICTURES,TEXTURES ETC **/
    loadSounds(&player);
    initMenu(&menu,&player);
    initPick(&pick);
    initLedges(&player);
    initCd(&player);
    SDLNet_Init();
    while(!exit) ///**** MAIN MENU ****/
    {
        if(LINUX)
            Mix_PlayMusic(player.sounds.backgroundLinux, -1);
        else
            Mix_PlayMusic(player.sounds.backgroundSound,-1);

        displayMenu(menu);
        enterIPmenu = handleMenu(&exit);

        while(enterIPmenu) /***** SKRIV IN IP ****/
        {
            pickCharacter = enterIP(&player);
            SDL_StopTextInput();
            if(pickCharacter)
            {
                if(!(networkInit(&client,&player,tmp)))
                {
                    ingame = 0;
                    pickCharacter = 0;
                    enterIPmenu = 0;
                }
                else
                {
                    player.connected = 1;
                    player.disconnected = 0;
                }
            }
            else
                enterIPmenu = 0;

            while(pickCharacter) /**** PICK CHARACTER ****/
            {
                displayMenu(pick);
                ingame = handlePick(&pickCharacter,&player);
                if(ingame==1)
                {
                    initPlayer(&player);
                    clearCartridge(ammo);
                    send_data(&player,&client,2);
                    player.deltaTimeMs = 0;
                    player.alive = 0;
                }
                recv_data(&player,&client,&done,ammo);
                while(ingame==1) /**** INGAME ****/
                {

                    if(player.deltaTimeMs < 1)
                    {
                        frameStart = SDL_GetTicks();
                        SDL_Delay(1);
                        frameEnd = SDL_GetTicks();
                        player.deltaTimeMs = frameEnd-frameStart;
                    }
                    frameStart = SDL_GetTicks();
                    player.deltaTimeS = (float)(player.deltaTimeMs)/ 1000;

                    direct = 0;
                    done = 0;
                    done = processEvents(&player,ammo,&moved,&type,&direct,&client);
                    updateEnemyBullet(&player);
                    updateLogic(&player,ammo);
                    if(player.alive)
                        collisionDetect(&player, &direct, &moved, &type);
                    bulletGone(ammo,&player,&client);

                    if(moved && player.alive)
                    {
                        send_data(&player,&client,type);
                        moved = 0;
                    }

                    if (done != 1)
                        recv_data(&player,&client,&done,ammo);

                    doRender(&player,ammo); //,&enemies[i]
                    detectHit(&player,ammo,&client);

                    if(!player.alive) /** RESPAWNING IN 4 SECONDS **/
                    {
                        currentTime = SDL_GetTicks();
                        currentTime = currentTime/1000;
                        if(currentTime>lastTime)
                        {
                            spawnTimer--;
                        }
                        lastTime = currentTime;
                        if(spawnTimer<= 0)
                        {
                            respawn(&player);
                            player.alive = 1;
                            send_data(&player,&client,2);
                            spawnTimer = 4;
                        }
                    }

                    if(done || player.disconnected)
                    {
                        ingame = 0;
                        if(LINUX)
                            Mix_PlayMusic(player.sounds.backgroundLinux, -1);
                        else
                            Mix_PlayMusic(player.sounds.backgroundSound,-1);
                    }
                    frameEnd = SDL_GetTicks();
                    player.deltaTimeMs = frameEnd - frameStart;
                }

                if(ingame==-1 || player.disconnected)
                {
                    if(!player.disconnected)
                    {
                        send_data(&player,&client,3);
                        SDL_Delay(1000);
                    }
                    SDLNet_FreeSocketSet(client.udpset);
                    SDLNet_FreeSocketSet(client.tcpset);
                    SDLNet_UDP_Close(client.udpsock);
                    SDLNet_TCP_Close(client.tcpsock);
                    pickCharacter = 0;
                    enterIPmenu = 0;
                }
                exit = 0;
            }
        }
    }
    TTF_Quit();
    if(LINUX)
    {
        Mix_FreeMusic(player.sounds.backgroundLinux);
    }
    else
    {
        Mix_FreeMusic(player.sounds.backgroundSound);
    }
    Mix_CloseAudio();
    free(tmp);
    SDLNet_Quit();
    Quit();

    // Close and destroy the window

    return 0;
}

