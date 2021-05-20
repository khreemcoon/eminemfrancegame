#pragma once

#include <iostream>
#include <SDL2/SDL.h>

class Timer{
private:
    unsigned int lastTime = 0, currentTime;
    int timeMS = 0;
    bool shot = false, permshot = false;
    bool oneshot;
public:
    Timer(int pTimeMs, bool pOneshot){
        timeMS = pTimeMs;
        oneshot = pOneshot;
    }
    bool getShot(){return shot;}
    void noShot(){shot = false;}

    void Update(){
        if(!permshot){
            currentTime = SDL_GetTicks();
            if(currentTime > lastTime + timeMS){
                shot = false;
                lastTime = currentTime;
                if(oneshot){
                    permshot = true;
                }
                shot = true;
            }
        }
        
    }
};