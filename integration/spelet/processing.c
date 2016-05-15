#include "processing.h"

extern int addBullet(int,int,int,Bullet b[],int b1,int b2);
extern void sendBullet(Player man,Network client);

void updateLogic(Player *p,Bullet b[])
{
    int i;

    for(i=0; i<20; i++)
    {
        if(b[i].active == 1)
        {
            b[i].x +=b[i].vector_unitX*6;
            b[i].y +=b[i].vector_unitY*6;
        }
    }
    global++;
}

void updateEnemyBullet(Player *man)
{
    int i,j;
    for(i=0;i<5;i++)
    {
        for(j=0;j<20;j++)
        {
            if(man->enemies[i].bullet[j].active == 1)
            {
                man->enemies[i].bullet[j].x += man->enemies[i].bullet[j].vector_unitX*6;
                man->enemies[i].bullet[j].y += man->enemies[i].bullet[j].vector_unitY*6;
            }
        }
    }
}
int processEvents(Player *man,Bullet b[],int *moved,int *type,int *direct,Network *client)
{
    Mix_Chunk *bulletShot = Mix_LoadWAV("bulletPop.WAV");
    unsigned int spellOne, spellOne_False=0;
    spellOne = SDL_GetTicks();
    SDL_Event event;
    int done = 0,shooting = 0;;
    int mouse1 = 0;

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
                *moved = 1;
                *type = 3;
            }
        }
        break;
        case SDL_KEYDOWN:
        {
            switch(event.key.keysym.sym)
            {
            case SDLK_ESCAPE:
                done = 1;
                *moved = 1;
                *type = 3;
                break;
            case SDLK_RETURN:
                done = 1;
                *moved = 1;
                *type = 3;
                break;
            }
        }
        break;
        case SDL_QUIT:
            //quit out of the game
            done = 1;
            *moved = 1;
            *type = 3;
            break;
        case SDL_MOUSEBUTTONDOWN :
            printf("clicked on mouse");
            mouse1 = 1;
            int blinkX,blinkY;
            SDL_GetMouseState(&blinkX, &blinkY);
            printf("Cursor at %d x %d\n",blinkX,blinkY);
        }
    }

    const Uint8 *state = SDL_GetKeyboardState(NULL);
    if(state[SDL_SCANCODE_LEFT] || state[SDL_SCANCODE_A] && man->alive)
    {
        man->x -= 5;
        *moved = 1;
        *type = 2;
        *direct += 1;
        if(man->frameX == 192)
        {
            man->frameX = 224;
        }
        else
        {
            man->frameX = 192;
        }
        if(man->x < 0)
            man->x = 0;
    }
    if(state[SDL_SCANCODE_RIGHT] || state[SDL_SCANCODE_D] && man->alive)
    {
        man->x += 5;
        *moved = 1;
        *type = 2;
        *direct += 2;
        if(man->frameX == 128)
        {
            man->frameX = 160;
        }
        else
        {
            man->frameX = 128;
        }
        if(man->x > 960)
            man->x = 960;

    }
    if(state[SDL_SCANCODE_UP] || state[SDL_SCANCODE_W] && man->alive)
    {
        man->y -= 5;
        *moved = 1;
        *type = 2;
        *direct += 4;
        if(man->frameX == 64)
        {
            man->frameX= 96;
        }
        else
        {
            man->frameX = 64;
        }
        if(man->y<0)
            man->y = 0;
    }
    if(state[SDL_SCANCODE_DOWN] || state[SDL_SCANCODE_S] && man->alive)
    {
        man->y += 5;
        *moved = 1;
        *type = 2;
        *direct += 8;
        if(man->frameX == 0)
        {
            man->frameX = 32;
        }
        else
        {
            man->frameX = 0;
        }
        if(man->y>566)
            man->y = 566;
    }
    if(mouse1 == 1 || state[SDL_SCANCODE_SPACE] && man->alive && !shooting)
    {
        int blinkX,blinkY,bulletNo;
        mouse1 = 0;
        SDL_GetMouseState(&blinkX, &blinkY);
        if(global%6==0)
        {
            shooting = 1;
            if(((bulletNo = addBullet(man->x,man->y,5,b,blinkX,blinkY))>=0))
            {
                Mix_PlayChannel(-1,bulletShot,0);
                man->blinkX = blinkX;
                man->blinkY = blinkY;
                man->bulletNo = bulletNo;
                if(man->connected)
                    sendBullet(*man,*client);
            }
            shooting = 0;
        }
    }
    checkCd(&*man);
    if(state[SDL_SCANCODE_1] && man->alive)
    {
        printf("pressed 1\n");
        *direct = -1;
        int bX,bY;

        //man->currentTime = SDL_GetTicks();
        if(spellOne > spellOne_False+1000 && man->spellReady) //man->currentTime > man->lastTime+3000
        {
            man->x1 = man->x;
            man->y1 = man->y;
            spellOne_False=spellOne-1;
            SDL_GetMouseState(&bX, &bY);
            float v_length = sqrt(((bX- man->x)*(bX-man->x))+((bY-man->y)*(bY-man->y)));
            Vector2d unit_vector;
            Vector2d direction;
            direction.x = bX-man->x;
            direction.y = bY-man->y;
            unit_vector.x = (direction.x)/ v_length;
            unit_vector.y = (direction.y)/ v_length;
           // SDL_Delay(150);
            man->x+=(unit_vector.x*100)-16;
            man->y+=(unit_vector.y*100)-16;
            *moved = 1;
            *type = 2;
            //man->lastTime = man->currentTime;
            man->blinkRect.w = 0;
            man->spellReady = 0;
        }
    }
    //printf("Thinktime : %d \n",man->thinkTime);
    return done;
}

void checkCd(Player *man)
{
    man->currentTime = SDL_GetTicks();
    if (man->currentTime > man->cdTime+40 && man->blinkRect.w < 150)  // && man->currentTime+3000 < man->lastTime
    {
        man->blinkRect.w += 2;
        man->cdTime = man->currentTime;
    }
    else if (man->blinkRect.w >= 150)
        man->spellReady = 1;
}

void collisionDetect(Player *man, int *direct)
{
    if (*direct > 0)
    {
        int i, bpe = 0;
        // check for collision with any ledges and enemies
        for (i = 0; i < 4; i++)
        {
            int mw = 64, mh = 64;
            int mx = man->x, my = man->y;

            // ladda ledges
            int bx = man->ledges[i].x, by = man->ledges[i].y, bw = man->ledges[i].w, bh = man->ledges[i].h;

            // kolla [i] för ledges och fiende
            while (bpe < 2)
            {
                if (my+mh > by && mx < bx+bw && mx+mw > bx && my < by+bh)
                {
                    // man moving left
                    if (*direct == 1 || *direct == 13)
                        man->x += 5;
                    // man moving right
                    else if (*direct == 2 || *direct == 14)
                        man->x -= 5;
                    // man moving up
                    else if (*direct == 4 || *direct == 7)
                        man->y += 5;
                    // man moving down
                    else if (*direct == 8 || *direct == 11)
                        man->y -= 5;

                    /**** DIAGONALT ****/
                    // man moving left up
                    else if (*direct == 5 && my < by+bh-6)
                        man->x += 5;
                    else if (*direct == 5 && my > by+bh-6)
                        man->y += 5;
                    // man moving right up
                    else if (*direct == 6 && my < by+bh-6)
                        man->x -= 5;
                    else if (*direct == 6 && my > by+bh-6)
                        man->y += 5;
                    // man moving left down
                    else if (*direct == 9 && my+mh > by+6)
                        man->x += 5;
                    else if (*direct == 9 && my+mh < by+6)
                        man->y -= 5;
                    // man moving right down
                    else if (*direct == 10 && my+mh > by+6)
                        man->x -= 5;
                    else if (*direct == 10 && my+mh < by+6)
                        man->y -= 5;
                }
                // ladda enemies istället för ledges
                //bx = man->enemies[i].dstRect.x, by = man->enemies[i].dstRect.y, bw = man->enemies[i].dstRect.w, bh = man->enemies[i].dstRect.h;
                bpe++;
            }
            bpe = 0;
        }
    }
    else if (*direct < 0)
    {
        int i, bpe = 0;
        int mw = 64, mh = 64;
        int mx = man->x+mw/2, my = man->y+mh/2;
        int ox = man->x1+mw/2, oy = man->y1+mh/2;

        // spelaren kan inte blinka utanför kartan
        if ((mx-mw/2) < 0)
            man->x = 0;
        if ((mx+mw/2) > 1024)
            man->x = 1024-mw;
        if ((my-mh/2) < 0)
            man->y = 0;
        if ((my+mh/2) > 630)
            man->y = 630-mh;

        // check for collision with any ledges and enemies
        for (i = 0; i < 4; i++)
        {
            // ladda ledges
            int bw = man->ledges[i].w, bh = man->ledges[i].h;
            int bx = man->ledges[i].x+bw/2, by = man->ledges[i].y+bh/2;

            // kolla [i] för ledges och fiende
            while (bpe < 2)
            {
                if ((my-mh/2)+mh > (by-bh/2) && (mx-mw/2) < (bx-bw/2)+bw && (mx-mw/2)+mw > (bx-bw/2) && (my-mh/2) < (by-bh/2)+bh)
                {
                    // höger sida
                    if (ox >= bx)
                    {
                        if (abs(bx-ox) > abs(by-oy))
                        {
                            man->x = bx+bw/2;
                        }
                        // över
                        else if (abs(oy-mh/2) < abs(by-bh/2))
                        {
                            man->y = by-bw/2-mh+8;
                        }
                        // under
                        else
                        {
                            man->y = by+bh/2+4;
                        }
                    }
                    // vänster sida
                    else if (ox <= bx)
                    {
                        if (abs(bx-ox) > abs(by-oy))
                        {
                            man->x = bx-bw/2-mw;
                        }
                        // över
                        else if (abs(oy-mh/2) < abs(by-bh/2))
                        {
                            man->y = by-bw/2-mh+8;
                        }
                        // under
                        else
                        {
                            man->y = by+bh/2+4;
                        }
                    }

                }
                // ladda enemies istället för ledges
               // if(man->enemies[i].alive)
               // {
                //    bw = man->enemies[i].dstRect.w, bh = man->enemies[i].dstRect.h;
               //     bx = man->enemies[i].dstRect.x+bw/2, by = man->enemies[i].dstRect.y+bh/2;
              //  }

                bpe++;
            }
            bpe = 0;
        }
    }
}

void bulletDetect(Player *man, Bullet b[])
{
    printf("asd\n");
}

void doRender(Player *man,Bullet b[]) //, Enemy *enemies
{
    int i,j;
    //set the drawing color to blue
    SDL_SetRenderDrawColor(program.renderer, 0, 0, 255, 255);
    //Clear the screen (to blue)
    SDL_RenderClear(program.renderer);

    SDL_Rect rect = { man->x, man->y, 32, 32 };
    SDL_Rect src = {man->frameX,0,32,32};
    SDL_Rect bg = {0,0,1024,768};
    //SDL_Rect scoreBg = {0,630,1024,138};

    SDL_RenderCopy(program.renderer,man->background,NULL,&bg);


    //SDL_Rect rectE = {enemies->dstRect.x, enemies->dstRect.y, 32, 32};
    //SDL_Rect srcE = {enemies->srcRect.x, 0, 32, 32};

    //SDL_RenderFillRect(program.renderer, &rect);


    for(i=0; i<20; i++)
    {
        if(b[i].active == 1)
        {

            SDL_Rect faggot = {b[i].x , b[i].y,8,8};
            SDL_RenderCopy(program.renderer,man->bullet,NULL,&faggot);

           // SDL_Rect faggot = {b[i].x , b[i].y,8,8 };
          //  printf("faggot x: %d faggot y: %d\n",faggot.x,faggot.y);
           // SDL_RenderCopyEx(program.renderer,man->bullet,NULL,&faggot,0,NULL,0);

        }
    }
    for(i=0;i<5;i++)
    {
        for(j=0;j<20;j++)
        {
            if(man->enemies[i].bullet[j].active == 1)
            {
                SDL_Rect enemybullet = {man->enemies[i].bullet[j].x,
                                        man->enemies[i].bullet[j].y,8,8};
               // printf("enemybullet x: %d, enemybullet y: %d\n",man->enemies[i].bullet[j].x,man->enemies[i].bullet[j].y);
                SDL_RenderCopy(program.renderer,man->bullet,NULL,&enemybullet);
            }
        }
    }

    if(man->alive)
    {
        SDL_Rect rect = { man->x, man->y, 64, 64 };
        SDL_Rect src = {man->frameX,0,30,32};

        SDL_RenderCopy(program.renderer,man->texture,&src,&rect);
    }

    for(i=0; i<10; i++)
    {
        if (man->enemies[i].exists && man->enemies[i].alive)
        {
            man->enemies[i].srcRect.w = 30;
            SDL_RenderCopyEx(program.renderer,man->enemies[i].texture, &man->enemies[i].srcRect, &man->enemies[i].dstRect, 0, NULL, 0);
        }
    }
    SDL_Rect scoreBg = {0,630,1024,138};
    SDL_RenderCopy(program.renderer,man->scoreBackground,NULL,&scoreBg);

    // cd bar för blink
    SDL_Rect blinkRec = {man->blinkRect.x, man->blinkRect.y, man->blinkRect.w, man->blinkRect.h};
    SDL_RenderCopy(program.renderer,man->cdTimer,NULL,&blinkRec);

    for(i=0; i<3; i++)
    {
        SDL_Rect ledgeRect = {man->ledges[i].x, man->ledges[i].y, man->ledges[i].w, man->ledges[i].h};
        SDL_RenderCopy(program.renderer, man->ledges[i].texture, NULL, &ledgeRect);
    }


    SDL_RenderPresent(program.renderer);
}
