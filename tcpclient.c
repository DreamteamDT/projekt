#include <stdio.h>
#include <stdlib.h>
#ifndef _MSC_VER
#include <unistd.h>
#endif
#include <string.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_net.h>

struct player{
  int xpos,ypos;
};

int main(int argc, char **argv)
{
  IPaddress ip;
  char tmp[1024];
  int id;
  Uint16 port;
  SDLNet_SocketSet socket=SDLNet_AllocSocketSet(30);
  /* initialize SDL */
  SDL_Init(0);
    
  /* initialize SDL_net */
  SDLNet_Init();
  
  /* get the port from the commandline */
  port=(Uint16) strtol(argv[2],NULL,0);
  
  /* Resolve the argument into an IPaddress type */
  SDLNet_ResolveHost(&ip,argv[1],port);
    
  /* open the server socket */
  TCPsocket sock = SDLNet_TCP_Open(&ip);
  SDLNet_TCP_Recv(sock,tmp,strlen(tmp)+1);
  puts(tmp);
  printf("Connected to the server!\n");
  
  SDLNet_TCP_AddSocket(socket,sock);
  int val;
  printf("Lyssna (1) eller skriva(2): ");
  scanf("%d",&val);
  if(val==1){
    while(1){
      SDLNet_TCP_Recv(sock,tmp,strlen(tmp)+1);
      puts(tmp);
    }
  }
  else if(val==2){
    while(1){
      printf("Skriv nagot: ");
      fgets(tmp,1024,stdin);
      SDLNet_TCP_Send(sock,tmp,strlen(tmp)+1);
    }
  }
  
  
  
  SDLNet_TCP_Close(sock);
  
  /* shutdown SDL_net */
  SDLNet_Quit();
  
  /* shutdown SDL */
  SDL_Quit();
  
  return(0);
}
