#include "definition.h"


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

int networkInit(Network *client,Player *man)
{
    SDLNet_Init();
    client->tcpset=SDLNet_AllocSocketSet(10);
    client->udpset=SDLNet_AllocSocketSet(10);
    IPaddress ip,tcpip;
    char tmp[1024];

    if(!(SDLNet_ResolveHost(&ip,"localhost",5000)))
    {
        printf("Couldnt resolve udp host\n");
    }
    if(!(SDLNet_ResolveHost(&tcpip,"localhost",4000)))
    {
        printf("Couldnt resolve tcp host\n");
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
    sscanf(tmp,"%d",&(man->id));
    printf("my ID: %d\n",man->id);

    SDLNet_TCP_AddSocket(client->tcpset,client->tcpsock);
    SDLNet_UDP_AddSocket(client->udpset,client->udpsock);

    return 1;
}

void send_data(Player *man,Network *client,int type)
{
    char tmp[128];

    if(type == 2)
    {
        sprintf(client->sendpack->data,"%d %d %d %d %d",type,man->id,man->x,man->y, man->frameX);
        SDLNet_UDP_Send(client->udpsock,-1,client->sendpack);
    }
    if(type == 3)
    {
        sprintf(tmp,"%d %d \n",type,man->id);
        int size=0;
        int len=strlen(tmp)+1;
        while(size<len)
        {
            size+=SDLNet_TCP_Send(client->tcpsock,tmp+size,len-size);
        }
        printf("Disconnected!\n");
    }
}

void recv_data(Player *man, Network *client)
{
     while(SDLNet_CheckSockets(client->udpset,0)>0)
     {
       char tmp[1024];
       int offset = 0;
       int type, enemyid, enemyDX, enemyDY, enemySX;
       do{
	     SDLNet_UDP_Recv(client->udpsock,tmp);
       } while(uncomplete_string(tmp));

       sscanf(tmp,"%d %d %d %d %d",&type,&enemyid,&enemyDX,&enemyDY,&enemySX);
       man->enemies[enemyid].x = enemyDX;
       //enemies[enemyid].y = enemyDY;

       //Om ny fiende
       if (!man->enemies[enemyid].exists)
       {

         man->enemies[enemyid].srcRect.x = 32;
         man->enemies[enemyid].srcRect.y = 0;
         man->enemies[enemyid].srcRect.w = 32;
         man->enemies[enemyid].srcRect.h = 32;

         man->enemies[enemyid].dstRect.x = 120;
         man->enemies[enemyid].dstRect.y = 140;
         man->enemies[enemyid].dstRect.w = 32;
         man->enemies[enemyid].dstRect.h = 32;

         man->enemies[enemyid].exists = 1;
         //enemies[enemyid].id = enemyid;
         send_data(&man,&client,2);
       }
       if (type == 2)
       {
           man->enemies[enemyid].dstRect.x = enemyDX;
           man->enemies[enemyid].dstRect.y = enemyDY;
           man->enemies[enemyid].srcRect.x = enemySX;
       }
       if(type == 3)
       {
	     // flyttar spelaren av skärmen
         //man->enemies[enemyid].dstRect.x = 1000;
         //man->enemies[enemyid].dstRect.y = 1000;
         //doRender(renderer, &gameState);
         man->enemies[enemyid].exists = 0;
       }
       /*if(type == 4)
       {
           gameState.enemies[enemyid].bul = 1;
           gameState.enemies[enemyid].bulRect.x = enemyX + BAT_WIDTH + 4;
           gameState.enemies[enemyid].bulRect.y = enemyY + BAT_HEIGHT/2;
           gameState.enemies[enemyid].bulRect.w = BUL_WIDTH;
           gameState.enemies[enemyid].bulRect.h = BUL_HEIGHT;
           printf("type 4 recv\n");
       }*/
     }
}
