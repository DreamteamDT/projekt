#include "bullet.h"
extern void sendBullets(Player *man,Bullet b[],Network *client);

SDL_Texture *getBulletSprite()
{
    return bullet.texture;
}
int loadAmmo(Bullet b[])
{
    int i;

    for(i=0; i< 20 ; i++)
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
    for(i=0; i<20; i++)
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

int detectHit(Player *man,Bullet b[])
{
    int i,j,k;
    for(i=0; i<5; i++)
    {
        if(man->enemies[i].exists)
            for(j=0; j<20; j++)
            {
                if(b[j].active)
                {

                    //  printf("manx: %d manx+32: %d",man->enemies[i].dstRect.x,man->enemies[i].dstRect.x+32);
                    if(b[j].x > (man->enemies[i].dstRect.x) && b[j].x <= (man->enemies[i].dstRect.x+32)
                            && b[j].y >= (man->enemies[i].dstRect.y) && b[j].y <= (man->enemies[i].dstRect.y+32))
                    {
                        b[j].active = 0;
                        man->recentHit = j;
                        man->hitid = i;
                        man->enemies[i].exists = 0;
                        man->enemies[i].texture = NULL;
                        return 1;
                    }

                }
            }
    }
    return 0;
}

void bulletGone(Bullet b[],Player *man,Network *client)
{
    int i, j;
    for(i=0; i<20; i++)
    {
        if(b[i].active)
        {
            if(b[i].x < (0-8) || b[i].x > 1032 || b[i].y < (0-8) || b[i].y > 638)
            {
                b[i].active = 0;

                int type = 9;

                sprintf(client->sendpack->data,"%d %d %d %d %d",
                        type,man->id,(int)b[i].x,(int)b[i].y,i);
                SDLNet_UDP_Send(client->udpsock,-1,client->sendpack);

            }
        }
    }

}

void bulletClear(Bullet b[],Player *man, Network *client)
{
    int i = man->recentHit;
    int type = 9;
    sprintf(client->sendpack->data,"%d %d %d %d %d",
            type,man->id,(int)b[i].x,(int)b[i].y,i);
    SDLNet_UDP_Send(client->udpsock,-1,client->sendpack);
}




