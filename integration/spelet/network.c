#include <stdio.h>
#include <stdlib.h>
#include "network.h"
#include <SDL.h>
#include <SDL_net.h>

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


