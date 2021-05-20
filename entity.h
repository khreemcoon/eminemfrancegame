#pragma once

#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

class Entity{
private:
	float x,y;
	int w, h;
	SDL_Texture* tex;
public:
	Entity(float pX, float pY, SDL_Texture* pTex);
	float getX(){return x;}
	float getY(){return y;}
	float setX(float newX){x = newX;return x;}
	float setY(float newY){y = newY;return y;}
	int getW();
	int getH();
	
	bool Collision(int x1, int y1, int w1, int h1, int x2, int y2, int w2, int h2);
	
	void setRandPos();

	SDL_Texture* getTex(){return tex;}
	void setTex(SDL_Texture* tex);
};
