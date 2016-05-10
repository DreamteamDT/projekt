#include "vector.h"

void doVector(Bullet *pv, Bullet b[])
{
    int i;

    for(i=0;i<20;i++)
    {
        pv = &b[i];
        if(pv->active == 1)
        {
           pv-> action();
        }
    }
}
void drawVector(Bullet *pv, Bullet b[])
{
  int i;
   for(i=0;i<20;i++)
   {
       pv = &b[i];
       if(pv->active == 1)
       {
          pv-> draw();
       }
   }

}
