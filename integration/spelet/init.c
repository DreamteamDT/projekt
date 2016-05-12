#include "init.h"
#include <stdio.h>

void init(Player *player, Ledge *ledge)
{

    SDL_Surface *ledg = IMG_Load("pelare.PNG");
    SDL_Texture *ltexture;
    ltexture = SDL_CreateTextureFromSurface(program.renderer,ledg);
    SDL_FreeSurface(ledg);
    int i, lx = 70, ly = 400;
    /*for (i = 0; i < 3; i++)
    {

        player->ledges[i].w = 60;
        player->ledges[i].h = 60;
        player->ledges[i].x = 220;
        player->ledges[i].y = ly;
        lx += 170;
        ly -= 130;
        printf("| %d |\n", player->ledges[i].x);
    }*/

    ledge->x = 320;
    ledge->y = 240;
    ledge->w = 60;
    ledge->h = 60;
    ledge->ltexture = ltexture;
    printf("| %d |\n", ledge->x);


//=======
//>>>>>>> 4fd6c0fd942b4c795ba0bcb258b485843f18957f
    SDL_Surface *image = IMG_Load("USA.PNG");
    SDL_Texture *texture;
    texture = SDL_CreateTextureFromSurface(program.renderer,image);
    SDL_FreeSurface(image);
    player->x = 220;
    player->y = 140;
    player->frameX = 0;
    player->frameY = 0;
    player->texture = texture;



}
SDL_Texture *initBullet()
{
    SDL_Surface *image = IMG_Load("bullet.PNG");
    SDL_Texture *texture;
    texture = SDL_CreateTextureFromSurface(program.renderer,image);
    return texture;
}
void Quit()
{
    SDL_DestroyWindow(program.window);
    SDL_DestroyRenderer(program.renderer);

    // Clean up
    SDL_Quit();

}
