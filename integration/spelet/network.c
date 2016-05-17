#include "definition.h"
extern void addEnemyBullet(int x,int y,int dx,Bullet b[],int b1,int b2,int i);
void checkRunningEnemyDirection(Player *man, int *bulletX, int *bulletY, int id);
extern  void generateScoreboard(Player *man);


int uncomplete_string(char tmp[])
{
    int i=0;
    while(tmp[i]!='\0')
    {
        if(tmp[i]=='\n')
            return 0;
        i++;
    }
    return 1;
}

int networkInit(Network *client,Player *man,char *ipaddress)
{
    int i,type;
    client->tcpset=SDLNet_AllocSocketSet(10);
    client->udpset=SDLNet_AllocSocketSet(10);
    IPaddress ip,tcpip;
    char tmp[1024];

    if((SDLNet_ResolveHost(&ip,man->ip,5000))==-1)
    {
        printf("Couldnt resolve udp host\n");
        return 0;
    }
    if((SDLNet_ResolveHost(&tcpip,man->ip,4000))==-1)
    {
        printf("Couldnt resolve tcp host\n");
        return 0;
    }

    if(!(client->udpsock = SDLNet_UDP_Open(0)))
    {
        printf("Couldnt open socket\n");
        return 0;
    }

    if(!(client->sendpack = SDLNet_AllocPacket(1024)))
    {
        printf("could not allocate packet\n");
        return 0;
    }

    if(!(client->rcvpack = SDLNet_AllocPacket(1024)))
    {
        printf("Could not allocate receiving packet\n");
        return 0;
    }
    client->sendpack->address.host = ip.host;
    client->sendpack->address.port = ip.port;
    client->sendpack->len = 50;

    //Försöker connecta till servern
    if(!(client->tcpsock = SDLNet_TCP_Open(&tcpip)))
    {
        printf("Couldnt connect to server\n");
        return 0;
    }
    SDLNet_TCP_Recv(client->tcpsock,tmp,1024);
    sscanf(tmp,"%d %d %f %f",&type,&(man->id),&man->x,&man->y);
    if(type==0)
        printf("my ID: %d\n",man->id);
    else if (type == 4)
    {
        printf("Server is full.\n");
        return 0;
    }
    man->alive = 1;

    SDLNet_TCP_AddSocket(client->tcpset,client->tcpsock);
    SDLNet_UDP_AddSocket(client->udpset,client->udpsock);


    return 1;
}

void send_data(Player *man,Network *client,int type)
{
    char tmp[128];
    int size,len;
    if(type == 2)
    {
        sprintf(client->sendpack->data,"%d %d %f %f %d %d",
                type,man->id,man->x,man->y, man->frameX,man->spritePick);
        SDLNet_UDP_Send(client->udpsock,-1,client->sendpack);
    }
    if(type == 3)
    {
        sprintf(tmp,"%d %d \n",type,man->id);
        size=0;
        len=strlen(tmp)+1;
        while(size<len)
        {
            size+=SDLNet_TCP_Send(client->tcpsock,tmp+size,len-size);
        }
        printf("Disconnected!\n");
    }
    if(type == 7)
    {
        sprintf(client->sendpack->data,"%d %d %d %d",type,man->id,man->hitid,man->bulletid);
        SDLNet_UDP_Send(client->udpsock,-1,client->sendpack);
    }

}

void sendBullet(Player man,Network client)
{
    int i,j,k,size,len;
    int type = 8;
    sprintf(client.sendpack->data,"%d %d %d %d %d %d %d",
            type,man.id,(int)man.x,(int)man.y, man.blinkX,man.blinkY,man.bulletNo);
    SDLNet_UDP_Send(client.udpsock,-1,client.sendpack);
}

void deathSound(Player *man)
{
    Mix_Chunk *deathsound1 = Mix_LoadWAV("Soundeffects\death14.WAV");
    Mix_Chunk *deathsound2 = Mix_LoadWAV("Soundeffects\death4.WAV");
    Mix_Chunk *deathsound3 = Mix_LoadWAV("Soundeffects\death15.WAV");
    Mix_Chunk *deathsound4 = Mix_LoadWAV("Soundeffects\death8.WAV");
     if(LINUX)
     {
         if(man->spritePick==1)
        {
           Mix_Chunk *deathsound = Mix_LoadWAV("Soundeffects\death14.wav");
           Mix_PlayChannel(-1,deathsound,0);
        }
        else if(man->spritePick==2)
        {
           Mix_Chunk *deathsound = Mix_LoadWAV("Soundeffects\death4.wav");
           Mix_PlayChannel(-1,deathsound,0);
        }
        else if(man->spritePick==3)
        {
           Mix_Chunk *deathsound = Mix_LoadWAV("Soundeffects\death15.wav");
           Mix_PlayChannel(-1,deathsound,0);
        }
        else if(man->spritePick==4)
        {
           Mix_Chunk *deathsound = Mix_LoadWAV("Soundeffects\death8.wav");
           Mix_PlayChannel(-1,deathsound,0);
        }
     }
    else
    {
        if(man->spritePick==1)
        {
            printf("test\n");

           Mix_PlayChannel(-1,deathsound1,0);
        }
        else if(man->spritePick==2)
        {

           Mix_PlayChannel(-1,deathsound2,0);
        }
        else if(man->spritePick==3)
        {

           Mix_PlayChannel(-1,deathsound3,0);
        }
        else if(man->spritePick==4)
        {

           Mix_PlayChannel(-1,deathsound4,0);
        }

    }

}


void recv_data(Player *man, Network *client,int *done,Bullet b[])
{

    int type, enemyid,enemySX,spritePick,hitid,bulletX,bulletY;
    float enemyDX,enemyDY;
    int blinkX,blinkY,bulletid;
    int kills,deaths;
    while(SDLNet_CheckSockets(client->udpset,0)>0)
    {
        SDLNet_UDP_Recv(client->udpsock,client->rcvpack);
        sscanf(client->rcvpack->data,"%d %d",&type,&enemyid);


        //man->enemies[enemyid].x = enemyDX;
        //man->enemies[enemyid].y = enemyDY;

        //Om ny fiende
        if (!man->enemies[enemyid].exists && (man->id!=enemyid))
        {
            sscanf(client->rcvpack->data,"%d %d %f %f %d %d",
                   &type,&enemyid,&enemyDX,&enemyDY,&enemySX,&spritePick);

            SDL_Surface *image;
            if(LINUX)
            {

                if(spritePick==1)
                {
                    image = IMG_Load("USA.png");
                    man->enemies[enemyid].sprite = 1;
                }
                else if(spritePick==2)
                {
                    image = IMG_Load("spriteRussia.png");
                    man->enemies[enemyid].sprite = 2;
                }
                else if(spritePick==3)
                {
                    image = IMG_Load("spriteMurica.png");
                    man->enemies[enemyid].sprite = 3;
                }
                else
                {
                    image = IMG_Load("spriteChina.png");
                    man->enemies[enemyid].sprite = 4;
                }

            }
            else
            {
                if(spritePick==1)
                {
                    image = IMG_Load("USA.PNG");
                    man->enemies[enemyid].sprite = 1;
                }
                else if(spritePick==2)
                {
                    image = IMG_Load("spriteRussia.PNG");
                    man->enemies[enemyid].sprite = 2;
                }
                else if(spritePick==3)
                {
                    image = IMG_Load("spriteMurica.PNG");
                    man->enemies[enemyid].sprite = 3;
                }
                else
                {
                    image = IMG_Load("spriteChina.PNG");
                    man->enemies[enemyid].sprite = 4;
                }
            }

            SDL_Texture *texture;
            texture = SDL_CreateTextureFromSurface(program.renderer,image);
            SDL_FreeSurface(image);
            man->enemies[enemyid].texture = texture;
            man->enemies[enemyid].srcRect.x = enemySX;
            man->enemies[enemyid].srcRect.y = 0;
            man->enemies[enemyid].srcRect.w = 32;
            man->enemies[enemyid].srcRect.h = 32;
            man->enemies[enemyid].dstRect.x = enemyDX;
            man->enemies[enemyid].dstRect.y = enemyDY;
            man->enemies[enemyid].dstRect.w = 64;
            man->enemies[enemyid].dstRect.h = 64;
            man->enemies[enemyid].exists = 1;
            man->enemies[enemyid].alive = 1;
            if(man->alive)
            {
                send_data(&*man,&*client,2);
            }

        }
        if (type == 2 && (SDL_GetTicks() - man->enemies[enemyid].justDied > 1000))
        {
            sscanf(client->rcvpack->data,"%d %d %f %f %d %d",
                   &type,&enemyid,&enemyDX,&enemyDY,&enemySX,&spritePick);
            man->enemies[enemyid].alive = 1;
            man->enemies[enemyid].dstRect.x = enemyDX;
            man->enemies[enemyid].dstRect.y = enemyDY;
            man->enemies[enemyid].srcRect.x = enemySX;
        }
        if(type == 7)
        {
            sscanf(client->rcvpack->data,"%d %d %d %d",&type,&enemyid,&hitid,&bulletid);
            if(hitid == man->id)
            {
                man->alive = 0;
                man->enemies[enemyid].bullet[bulletid].active = 0;
                deathSound(&*man);
            }
            else if(enemyid == man->id)
            {
                man->enemies[hitid].alive = 0;
                b[bulletid].active = 0;
                deathSound(&*man);
            }
            else
            {
                man->enemies[hitid].alive = 0;
                man->enemies[enemyid].bullet[bulletid].active = 0;
                deathSound(&*man);
            }
        }
        if (type == 8)
        {
            sscanf(client->rcvpack->data,"%d %d %d %d %d %d %d",
                   &type,&enemyid,&bulletX,&bulletY,&blinkX,&blinkY,&bulletid);
            checkRunningEnemyDirection(&*man, &bulletX, &bulletY, enemyid);
            addEnemyBullet(bulletX,bulletY,5,man->enemies[enemyid].bullet,blinkX,blinkY,bulletid);
        }
        if(type == 10)
        {
            sscanf(client->rcvpack->data,"%d %d %d %d",&type,&enemyid,&kills,&deaths);
            //printf("%d %d %d %d\n",type,enemyid,kills,deaths);
            if(enemyid == man->id)
            {
                man->kills = kills;
                man->deaths = deaths;
            }
            else
            {
                man->enemies[enemyid].kills = kills;
                man->enemies[enemyid].deaths = deaths;
            }
            generateScoreboard(&*man);

        }
    }

    while(SDLNet_CheckSockets(client->tcpset,0)>0)
    {
        printf("incoming data on tcp socket\n");
        int offset = 0;
        int max = 0;
        char tmp[1024];
        do
        {
            offset+=SDLNet_TCP_Recv(client->tcpsock,tmp+offset,1024);
            max++;
        }
        while(uncomplete_string(tmp) && max<20);
        sscanf(tmp,"%d %d",&type,&enemyid);
        if(max>=20)
        {
            type = 6;
        }

        if(type == 3)
        {
            man->enemies[enemyid].exists = 0;
            man->enemies[enemyid].texture = NULL;
        }

        if(type == 6)
        {
            printf("Server shut down!\n");
            *done = 1;
            return;
        }

    }
}
