#include "processing.h"

extern void addBullet(int,int,int,Bullet b[],int b1,int b2);

void updateLogic(Player *p,Bullet b[])
{
    int i;

    for(i=0; i<20; i++)
    {
        if(b[i].active == 1)
        {
            b[i].x +=b[i].vector_unitX*3;
            b[i].y +=b[i].vector_unitY*3;
        }
    }
    global++;
}

int processEvents(Player *man,Bullet b[],int *moved,int *type,int *direct)
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
            int blinkX,blinkY;
            SDL_GetMouseState(&blinkX, &blinkY);
            printf("Cursor at %d x %d\n",blinkX,blinkY);

        }
    }

    const Uint8 *state = SDL_GetKeyboardState(NULL);
    if(state[SDL_SCANCODE_LEFT] || state[SDL_SCANCODE_A])
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
    if(state[SDL_SCANCODE_RIGHT] || state[SDL_SCANCODE_D])
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
        if(man->x > 992)
            man->x = 992;

    }
    if(state[SDL_SCANCODE_UP] || state[SDL_SCANCODE_W])
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
    if(state[SDL_SCANCODE_DOWN] || state[SDL_SCANCODE_S])
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
        if(man->y>736)
            man->y = 736;
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
        *direct = -1;
        printf("|| dir %d\n", *direct);
        int bX,bY;
        if(spellOne > spellOne_False+1000)
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
            SDL_Delay(150);
            man->x+=(unit_vector.x*100)-16;
            man->y+=(unit_vector.y*100)-16;
        }
    }
    //printf("Thinktime : %d \n",man->thinkTime);
    return done;
}

void collisionDetect(Player *man, int *direct)
{
    if (*direct > 0)
    {
        int i, bpe = 0;
        // check for collision with any ledges and enemies
        for (i = 0; i < 3; i++)
        {
            int mw = 32, mh = 32;
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
                bx = man->enemies[i].dstRect.x, by = man->enemies[i].dstRect.y, bw = man->enemies[i].dstRect.w, bh = man->enemies[i].dstRect.h;
                bpe++;
            }
            bpe = 0;
        }
    }
    else if (*direct < 0)
    {
        int i, bpe = 0;
        // check for collision with any ledges and enemies
        for (i = 0; i < 3; i++)
        {
            int mw = 32, mh = 32;
            int mx = man->x+mw/2, my = man->y+mh/2;
            int ox = man->x1+mw/2, oy = man->y1+mh/2;
            // ladda ledges
            int bw = man->ledges[i].w, bh = man->ledges[i].h;
            int bx = man->ledges[i].x+bw/2, by = man->ledges[i].y+bh/2;


            // kolla [i] för ledges och fiende
            while (bpe < 2)
            {
                if ((my-mh/2)+mh > (by-bh/2) && (mx-mw/2) < (bx-bw/2)+bw && (mx-mw/2)+mw > (bx-bw/2) && (my-mh/2) < (by-bh/2)+bh)
                {
                    // höger sida
                    if (ox > bx)
                    {
                        if (abs(bx-ox) > abs(by-oy))
                        {
                            man->x = bx+bw/2;
                        }
                        else if (abs(oy+mh/2) < abs(by-bh/2))
                        {
                            man->y = by-bw/2-mh;
                        }
                        else
                        {
                            man->y = by+bh/2;
                        }
                    }
                    // vänster sida
                    else if (ox < bx)
                    {
                        if (abs(bx-ox) > abs(by-oy))
                        {
                            man->x = bx-bw/2-mw;
                        }
                        else if (abs(oy+mh/2) < abs(by-bh/2))
                        {
                            man->y = by-bw/2-mh;
                        }
                        else
                        {
                            man->y = by+bh/2;
                        }
                    }

                }
                // ladda enemies istället för ledges
                bw = man->enemies[i].dstRect.w, bh = man->enemies[i].dstRect.h;
                bx = man->enemies[i].dstRect.x+bw/2, by = man->enemies[i].dstRect.y+bh/2;
                bpe++;
            }
            bpe = 0;
        }
    }
}

void doRender(Player *man,Bullet b[]) //, Enemy *enemies
{
    int i;
    //set the drawing color to blue
    SDL_SetRenderDrawColor(program.renderer, 0, 0, 255, 255);
    //Clear the screen (to blue)
    SDL_RenderClear(program.renderer);

    SDL_Rect rect = { man->x, man->y, 32, 32 };
    SDL_Rect src = {man->frameX,0,32,32};
    SDL_Rect bg = {0,0,1024,768};
    SDL_Rect scoreBg = {0,630,1024,138};
    SDL_RenderCopy(program.renderer,man->background,NULL,&bg);
    SDL_RenderCopy(program.renderer,man->scoreBackground,NULL,&scoreBg);

    //SDL_Rect rectE = {enemies->dstRect.x, enemies->dstRect.y, 32, 32};
    //SDL_Rect srcE = {enemies->srcRect.x, 0, 32, 32};

    //SDL_RenderFillRect(program.renderer, &rect);
    SDL_RenderCopy(program.renderer,man->texture,&src,&rect);



    for(i=0; i<20; i++)
    {
        if(b[i].active == 1)
        {
            SDL_Rect faggot = {b[i].x , b[i].y,8,8 };
          //  printf("faggot x: %d faggot y: %d\n",faggot.x,faggot.y);
            SDL_RenderCopyEx(program.renderer,man->bullet,NULL,&faggot,0,NULL,0);
        }
    }
    for(i=0; i<10; i++)
    {
        if (man->enemies[i].exists)
        {
            SDL_RenderCopyEx(program.renderer,man->enemies[i].texture, &man->enemies[i].srcRect, &man->enemies[i].dstRect, 0, NULL, 0);
        }
    }
    for(i=0; i<3; i++)
    {
        //printf("| x: %d\n", man->ledges[i].x);
        SDL_Rect ledgeRect = {man->ledges[i].x, man->ledges[i].y, man->ledges[i].w, man->ledges[i].h};
        SDL_RenderCopy(program.renderer, man->ledges[i].texture, NULL, &ledgeRect);
        //printf("test\n");
    }


    SDL_RenderPresent(program.renderer);
}
