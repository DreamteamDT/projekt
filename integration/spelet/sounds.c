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
            man->sounds.Ldeathsound1 = Mix_LoadWAV("Soundeffects/deathtorgny.wav");
            man->sounds.Ldeathsound2 = Mix_LoadWAV("Soundeffects/death4.wav");
            man->sounds.Ldeathsound3 = Mix_LoadWAV("Soundeffects/death15.wav");
            man->sounds.Ldeathsound4 = Mix_LoadWAV("Soundeffects/death8.wav");
            man->sounds.bulletShot = Mix_LoadWAV("bulletPop.wav");

    }
    else
    {
            man->sounds.bulletShot = Mix_LoadWAV("bulletPop.WAV");
            man->sounds.backstreet = Mix_LoadMUS("Backstreet.MP3");
            man->sounds.cykablyat = Mix_LoadMUS("cykablyat.MP3");
            man->sounds.america = Mix_LoadMUS("America.MP3");
            man->sounds.china = Mix_LoadMUS("Chinese.MP3");
            man->sounds.deathsound1 = Mix_LoadWAV("Soundeffects/deathtorgny.WAV");
            man->sounds.deathsound2 = Mix_LoadWAV("Soundeffects/death4.WAV");
            man->sounds.deathsound3 = Mix_LoadWAV("Soundeffects/death15.WAV");
            man->sounds.deathsound4 = Mix_LoadWAV("Soundeffects/death8.WAV");
    }
}

void enemyDeathSound(Player *man, int hitid)
{



    if(LINUX)
    {
        if(man->enemies[hitid].sprite==1)
        {

           Mix_PlayChannel(-1,man->sounds.Ldeathsound1,0);
        }
        else if(man->enemies[hitid].sprite==2)
        {

           Mix_PlayChannel(-1,man->sounds.Ldeathsound2,0);
        }
        else if(man->enemies[hitid].sprite==3)
        {

           Mix_PlayChannel(-1,man->sounds.Ldeathsound3,0);
        }
        else if(man->enemies[hitid].sprite==4)
        {

           Mix_PlayChannel(-1,man->sounds.Ldeathsound4,0);
        }

    }
    else
    {
        if(man->enemies[hitid].sprite==1)
        {

           Mix_PlayChannel(-1,man->sounds.deathsound1,0);
        }
        else if(man->enemies[hitid].sprite==2)
        {

           Mix_PlayChannel(-1,man->sounds.deathsound2,0);
        }
        else if(man->enemies[hitid].sprite==3)
        {

           Mix_PlayChannel(-1,man->sounds.deathsound3,0);
        }
        else if(man->enemies[hitid].sprite==4)
        {

           Mix_PlayChannel(-1,man->sounds.deathsound4,0);
        }
    }
}


void playerDeathSound(Player *man)
{

     if(LINUX)
     {
         if(man->spritePick==1)
        {
           Mix_PlayChannel(-1,man->sounds.Ldeathsound1,0);
        }
        else if(man->spritePick==2)
        {
           Mix_PlayChannel(-1,man->sounds.Ldeathsound2,0);
        }
        else if(man->spritePick==3)
        {
           Mix_PlayChannel(-1,man->sounds.Ldeathsound3,0);
        }
        else if(man->spritePick==4)
        {
           Mix_PlayChannel(-1,man->sounds.Ldeathsound4,0);
        }
     }
    else
    {
        if(man->spritePick==1)
        {
           Mix_PlayChannel(-1,man->sounds.deathsound1,0);
        }
        else if(man->spritePick==2)
        {
           Mix_PlayChannel(-1,man->sounds.deathsound2,0);
        }
        else if(man->spritePick==3)
        {
           Mix_PlayChannel(-1,man->sounds.deathsound3,0);
        }
        else if(man->spritePick==4)
        {
           Mix_PlayChannel(-1,man->sounds.deathsound4,0);
        }

    }

}
