#include "bullet.h"

SDL_Texture *getBulletSprite()
{
    return bullet.texture;
}
int loadAmmo(Bullet b[])
{
     int i;

     for(i=0;i< 20 ; i++)
     {
        if(b[i].active == 0)
        {
            memset(&b[i], 0, sizeof(Bullet));
			b[i].active = 1;
			return i;
        }

     }
     return -1;

}
void shotgun(Player *p)
{
     if(p->shot == 1)
     {
        if(p->thinkTime <= 0)
          {
            printf("Shit!\n");
            p->thinkTime = 20;
          }
     }
}
/*
void addBullet(Bullet a[])
{
    a[1].y = 21;
    a[2].x = 9;
    a[2].y = 23;

}
*/
//void addBullet()

void clearCartridge(Bullet a[])
{
  int i;
     for(i=0;i<20;i++)
     {
         a[i].active = 0;
         memset(&a[i],0,sizeof(Bullet));
     }
}
void addBullet(int x, int y,int dx, Bullet b[],int b1,int b2)
{
     int i = loadAmmo(b);
     if(i== -1)
     {
         printf("couldn't get a free slot for a bullet!\n");
         return;
     }
     b[i].x = x;
     b[i].y = y;
     b[i].v_length = sqrt(((b1-b[i].x)*(b1-b[i].x)) + ((b2 - b[i].y)*(b2 - b[i].y)));

     b[i].directionX = b1 - b[i].x;
     b[i].directionY = b2 - b[i].y;

     b[i].vector_unitX = (b[i].directionX) / b[i].v_length;
     b[i].vector_unitY = (b[i].directionY) / b[i].v_length;

     //b[i].dx = dx;
    // printf("b1 : %d\nb2 : %d\n",b1,b2);
}
