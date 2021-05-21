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
    void setInterval(int pInterval){timeMS = pInterval;}

    void Update(){
        if(!permshot){
            shot = false;
            currentTime = SDL_GetTicks();
            if(currentTime > lastTime + timeMS){
                lastTime = currentTime;
                if(oneshot){
                    permshot = true;
                }
                shot = true;
            }
        }
        
    }
};