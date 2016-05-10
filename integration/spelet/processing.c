#include "processing.h"

extern void addBullet(int,int,int,Bullet b[],int b1,int b2);

void updateLogic(Player *p,Bullet b[])
{
    int i;

    for(i=0;i<20;i++)
    {
        if(b[i].active == 1)
      {
        b[i].x +=b[i].vector_unitX*3;
        b[i].y +=b[i].vector_unitY*3;
        }
    }
    global++;
}

int processEvents(Player *man,Bullet b[])
{
  unsigned int spellOne, spellOne_False=0;
  spellOne = SDL_GetTicks();
  SDL_Event event;
  int done = 0;

  man->thinkTime--;
  if(man->thinkTime<=0)
  {
      man->thinkTime = 0;
  }

  while(SDL_PollEvent(&event))
  {
    switch(event.type)
    {
      case SDL_WINDOWEVENT_CLOSE:
      {
        if(program.window)
        {
          SDL_DestroyWindow(program.window);
          program.window = NULL;
          done = 1;
        }
      }
      break;
      case SDL_KEYDOWN:
      {
        switch(event.key.keysym.sym)
        {
          case SDLK_ESCAPE:
            done = 1;
          break;
        }
      }
      break;
      case SDL_QUIT:
        //quit out of the game
        done = 1;
      break;
      case SDL_MOUSEBUTTONDOWN :
      printf("clicked on mouse");
      int blinkX,blinkY;
      SDL_GetMouseState(&blinkX, &blinkY);
      printf("Cursor at %d x %d\n",blinkX,blinkY);

    }
  }

  const Uint8 *state = SDL_GetKeyboardState(NULL);
  if(state[SDL_SCANCODE_LEFT])
  {
    man->x -= 10;
    if(man->frameX == 192)
		{
		    man->frameX = 224;
		}
		else
		{
		    man->frameX = 192;
		}

  }
  if(state[SDL_SCANCODE_RIGHT])
  {
    man->x += 10;
    if(man->frameX == 128)
		{
		   man->frameX = 160;
		}
		else
		{
		   man->frameX = 128;
		}

  }
  if(state[SDL_SCANCODE_UP])
  {
    man->y -= 10;
    if(man->frameX == 64)
		{
           man->frameX= 96;
		}
		else
		{
		   man->frameX = 64;
		}
  }
  if(state[SDL_SCANCODE_DOWN])
  {
    man->y += 10;
    if(man->frameX == 0)
		{
		   man->frameX = 32;
		}
		else
		{
		   man->frameX = 0;
		}
  }
  if(state[SDL_SCANCODE_SPACE])
  {
     int blinkX,blinkY;
     SDL_GetMouseState(&blinkX, &blinkY);
     man->shot = 1;
     if(global%6==0)
     {
       float dirX,dirY;
       dirX = blinkX - man->x;
       dirY = blinkY - man->y;

       addBullet(man->x,man->y,5,b,blinkX,blinkY);
     }
  }
  if(state[SDL_SCANCODE_1])
  {
     printf("pressed 1\n");
      int bX,bY;
      if(spellOne > spellOne_False+1000)
      {
      spellOne_False=spellOne-1;
      SDL_GetMouseState(&bX, &bY);
      float v_length = sqrt(((bX- man->x)*(bX-man->x))+((bY-man->y)*(bY-man->y)));
      Vector2d unit_vector;
      Vector2d direction;
      direction.x = bX-man->x;
      direction.y = bY-man->y;
      unit_vector.x = (direction.x)/ v_length;
      unit_vector.y = (direction.y)/ v_length;
      SDL_Delay(150);
      man->x+=(unit_vector.x*100)-16;
      man->y+=(unit_vector.y*100)-16;
     }
    }
  //printf("Thinktime : %d \n",man->thinkTime);
  return done;
}

void doRender(Player *man,Bullet b[])
{
  int i;
  //set the drawing color to blue
  SDL_SetRenderDrawColor(program.renderer, 0, 0, 255, 255);
  //Clear the screen (to blue)
  SDL_RenderClear(program.renderer);
  SDL_Rect rect = { man->x, man->y, 32, 32 };
  SDL_Rect src = {man->frameX,0,32,32};
  //SDL_RenderFillRect(program.renderer, &rect);
  SDL_RenderCopy(program.renderer,man->texture,&src,&rect);
  for(i=0;i<20;i++)
  {
     if(b[i].active == 1)
     {
        SDL_Rect faggot = {b[i].x , b[i].y,8,8 };
        SDL_RenderCopy(program.renderer,bullet.texture,NULL,&faggot);
     }
  }
  SDL_RenderPresent(program.renderer);
}
