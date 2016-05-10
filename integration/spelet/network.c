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

    if(!(SDLNet_ResolveHost(&ip,"127.0.0.1",5000)))
    {
        printf("Couldnt resolve udp host\n");
    }
    if(!(SDLNet_ResolveHost(&tcpip,"127.0.0.1",4000)))
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
