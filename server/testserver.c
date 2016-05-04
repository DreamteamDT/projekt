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
  int exists;
};

int main(int argc, char **argv)
{
  int maxPlayers = 10;
  int x,y,type,id,hej;
  IPaddress ip;
  char tmp[1024];
  int curid=0;
  int playernum=0;
	
  Uint32 ipaddr;
  Uint16 port;
  struct player players[maxPlayers];
  SDL_Init(0);
  SDLNet_Init();
  SDLNet_SocketSet sockets=SDLNet_AllocSocketSet(10);
  int i,k;

  for(i=0;i<maxPlayers;i++){ //initiera allt till 0
    players[i].exists = 0;
  }
  
  port=(Uint16)strtol(argv[1],NULL,0);
  
  /* Resolve the argument into an IPaddress type */
  SDLNet_ResolveHost(&ip,NULL,port);

  /* open the server socket */
  TCPsocket server=SDLNet_TCP_Open(&ip);
  int next=0;
  
  while(1){

    for(i=0;i<10;i++){ //Hittar första bästa lediga spot
      if(!players[i].exists){
	next = i;
	i = 10;
      }
    }
    // printf("%d \n",next);
    // Försöker acca connection
    players[next].socket = SDLNet_TCP_Accept(server);
    if(players[next].socket){
	  
      SDLNet_TCP_AddSocket(sockets,players[next].socket);
      players[next].exists = 1;
      sprintf(tmp,"%d",next);
      printf("New connection\n");
      SDLNet_TCP_Send(players[next].socket,tmp,strlen(tmp)+1);
    }

    //check for incoming data
    while(SDLNet_CheckSockets(sockets,0)>0){
      printf("check sockets\n");
      for(i=0;i<maxPlayers;i++){
	if(players[i].exists)
	  if(SDLNet_SocketReady(players[i].socket)){
	    printf("socket ready\n");
	    //	  printf("Inkommande paket\n");
	    if(!(SDLNet_TCP_Recv(players[i].socket,tmp,1024)))
	      {
		printf("SDLNet_TCP_Recv: %s\n", SDLNet_GetError());
		return 1;
	      }
	    else sscanf(tmp,"%d %d",&type,&id);
	    printf("inlast\n");

	    if(type==2){
	      for(k=0;k<maxPlayers;k++){
		if(players[k].exists){
		  if(k!=i){
		    SDLNet_TCP_Send(players[k].socket,tmp,strlen(tmp)+1);
		  }
		}
	      }
	    }
	    else if(type == 3){
	      for(k=0;k<maxPlayers;k++){
		if(players[k].exists){
		  
		  if(k!=i){
		    SDLNet_TCP_Send(players[k].socket,tmp,strlen(tmp)+1);
		  }
		}
	      }
	      SDLNet_TCP_DelSocket(sockets,players[id].socket);
	      SDLNet_TCP_Close(players[id].socket);
	      players[id].exists = 0;
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
