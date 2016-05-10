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
        sprintf(client->sendpack->data,"%d %d %d %d",type,man->id,man->x,man->y);
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
