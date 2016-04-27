#include <stdlib.h>
#ifndef _MSC_VER
#include <unistd.h>
#endif
#include <SDL2/SDL.h>
#include <SDL2/SDL_net.h>
#include <string.h>

struct player{
  TCPsocket socket;
  int id;
};

int main(int argc, char **argv)
{
  IPaddress ip;
  char tmp[1024];
  int curid=0;
  int playernum=0;
	
  Uint32 ipaddr;
  Uint16 port;
  struct player players[10];
  SDL_Init(0);
  SDLNet_Init();
  SDLNet_SocketSet sockets=SDLNet_AllocSocketSet(30);
  
  port=(Uint16)strtol(argv[1],NULL,0);
  
  /* Resolve the argument into an IPaddress type */
  SDLNet_ResolveHost(&ip,NULL,port);

  /* open the server socket */
  TCPsocket server=SDLNet_TCP_Open(&ip);

  while(1){
    /* try to accept a connection */
    players[curid].socket = SDLNet_TCP_Accept(server);
    if(players[curid].socket){
      
      /* get the clients IP and port number */
      // remoteip=SDLNet_TCP_GetPeerAddress(client);
      
      SDLNet_TCP_AddSocket(sockets,players[curid].socket);
      playernum++;
      sprintf(tmp,"0 %d",curid);
      printf("New connection\n");
      SDLNet_TCP_Send(players[curid].socket,tmp,strlen(tmp)+1);
      curid++;
    }

    //check for incoming data
    while(SDLNet_CheckSockets(sockets,0)>0){
      
      int i;
      for(i=0;i<curid;i++){
	if(SDLNet_SocketReady(players[i].socket)){
	  SDLNet_TCP_Recv(players[i].socket,tmp,strlen(tmp)+1);
	  puts(tmp);
	  int k;
	  for(k=0;k<curid;k++){
	    SDLNet_TCP_Send(players[k].socket,tmp,strlen(tmp)+1);
	  }
	}
      }
      
      SDL_Delay(1);
    }

    
  }
  
  SDLNet_TCP_Close(server);
  SDLNet_FreeSocketSet(sockets);
  /* shutdown SDL_net */
  SDLNet_Quit();
  
  /* shutdown SDL */
  SDL_Quit();
  
  return(0);
}
