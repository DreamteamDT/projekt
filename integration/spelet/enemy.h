struct enemies
{
    int id;
    int x,y;
    SDL_Surface* bitmap;
    int exists;
    SDL_Rect srcRect,dstRect;
}typedef struct enemies Enemies;
