#include "definition.h"

void loadSounds(Player *man)
{
    if(LINUX)
    {
        Mix_OpenAudio(44100,MIX_DEFAULT_FORMAT,2,2048);
        man->sounds.backgroundLinux = Mix_LoadMUS("Soundeffects/gta3.wav");

    }
    else
    {
        Mix_OpenAudio(44100,MIX_DEFAULT_FORMAT,2,2048);
        Mix_VolumeMusic(20);
        man->sounds.backgroundSound = Mix_LoadMUS("Soundeffects/gta3.MP3");
    }

    if(LINUX)
    {

            man->sounds.backstreet = Mix_LoadMUS("Backstreet.wav");
            man->sounds.cykablyat = Mix_LoadMUS("cykablyat.wav");
            man->sounds.america = Mix_LoadMUS("America.wav");
            man->sounds.china = Mix_LoadMUS("Chinese.wav");
    }
    else
    {
            man->sounds.backstreet = Mix_LoadMUS("Backstreet.MP3");
            man->sounds.cykablyat = Mix_LoadMUS("cykablyat.MP3");
            man->sounds.america = Mix_LoadMUS("America.MP3");
            man->sounds.china = Mix_LoadMUS("Chinese.MP3");

    }

    man->sounds.bulletShot = Mix_LoadWAV("bulletPop.WAV");
}

void enemyDeathSound(Player *man, int hitid)
{
    Mix_Chunk *deathsound1 = Mix_LoadWAV("Soundeffects/deathtorgny.WAV");
    Mix_Chunk *deathsound2 = Mix_LoadWAV("Soundeffects/death4.WAV");
    Mix_Chunk *deathsound3 = Mix_LoadWAV("Soundeffects/death15.WAV");
    Mix_Chunk *deathsound4 = Mix_LoadWAV("Soundeffects/death8.WAV");


    if(LINUX)
    {
        if(man->enemies[hitid].sprite==1)
        {
           Mix_Chunk *deathsound = Mix_LoadWAV("Soundeffects/deathtorgny.wav");
           Mix_PlayChannel(-1,deathsound,0);
        }
        else if(man->enemies[hitid].sprite==2)
        {
           Mix_Chunk *deathsound = Mix_LoadWAV("Soundeffects/death4.wav");
           Mix_PlayChannel(-1,deathsound,0);
        }
        else if(man->enemies[hitid].sprite==3)
        {
           Mix_Chunk *deathsound = Mix_LoadWAV("Soundeffects/death15.wav");
           Mix_PlayChannel(-1,deathsound,0);
        }
        else if(man->enemies[hitid].sprite==4)
        {
           Mix_Chunk *deathsound = Mix_LoadWAV("Soundeffects/death8.wav");
           Mix_PlayChannel(-1,deathsound,0);
        }

    }
    else
    {
        if(man->enemies[hitid].sprite==1)
        {

           Mix_PlayChannel(-1,deathsound1,0);
        }
        else if(man->enemies[hitid].sprite==2)
        {

           Mix_PlayChannel(-1,deathsound2,0);
        }
        else if(man->enemies[hitid].sprite==3)
        {

           Mix_PlayChannel(-1,deathsound3,0);
        }
        else if(man->enemies[hitid].sprite==4)
        {

           Mix_PlayChannel(-1,deathsound4,0);
        }
    }
}


void playerDeathSound(Player *man)
{
    Mix_Chunk *deathsound1 = Mix_LoadWAV("Soundeffects/deathtorgny.WAV");
    Mix_Chunk *deathsound2 = Mix_LoadWAV("Soundeffects/death4.WAV");
    Mix_Chunk *deathsound3 = Mix_LoadWAV("Soundeffects/death15.WAV");
    Mix_Chunk *deathsound4 = Mix_LoadWAV("Soundeffects/death8.WAV");
     if(LINUX)
     {
         if(man->spritePick==1)
        {
           Mix_Chunk *deathsound = Mix_LoadWAV("Soundeffects/deathtorgny.wav");
           Mix_PlayChannel(-1,deathsound,0);
        }
        else if(man->spritePick==2)
        {
           Mix_Chunk *deathsound = Mix_LoadWAV("Soundeffects/death4.wav");
           Mix_PlayChannel(-1,deathsound,0);
        }
        else if(man->spritePick==3)
        {
           Mix_Chunk *deathsound = Mix_LoadWAV("Soundeffects/death15.wav");
           Mix_PlayChannel(-1,deathsound,0);
        }
        else if(man->spritePick==4)
        {
           Mix_Chunk *deathsound = Mix_LoadWAV("Soundeffects/death8.wav");
           Mix_PlayChannel(-1,deathsound,0);
        }
     }
    else
    {
        if(man->spritePick==1)
        {
            printf("test\n");

           Mix_PlayChannel(-1,deathsound1,0);
        }
        else if(man->spritePick==2)
        {

           Mix_PlayChannel(-1,deathsound2,0);
        }
        else if(man->spritePick==3)
        {

           Mix_PlayChannel(-1,deathsound3,0);
        }
        else if(man->spritePick==4)
        {

           Mix_PlayChannel(-1,deathsound4,0);
        }

    }

}
