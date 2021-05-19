#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "entity.h"

class Player:public Entity{
private:
	int score = 0;
	bool dead = false;
	float clamp(float pMin, float pMax, float pVal);
public:
	
	const float SPEED = .40f;
	float xVel, yVel;
	Player(float pX, float pY, SDL_Texture* pTex);
	void Update();
	bool isDead(){return dead;}
	int getScore(){return score;}
	const char* getScoreChar();
	void addScore(int num){score += num;}
	void Reset();

};
