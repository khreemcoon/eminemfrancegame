#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "entity.h"

Entity::Entity(float pX, float pY, SDL_Texture* pTex){
	srand(time(NULL));
	x = pX;
	y = pY;
	tex = pTex;
}

int Entity::getW(){
	SDL_QueryTexture(tex, NULL, NULL, &w, NULL);	
	return w;
}

int Entity::getH(){
	SDL_QueryTexture(tex, NULL, NULL, NULL, &h);	
	return h;
}

void Entity::setTex(SDL_Texture* pTex){
	tex = pTex;
}

bool Entity::Collision(int x1,int y1,int w1,int h1,int x2,int y2,int w2, int h2){
	return  x1 < x2 + w2 and
		y1 < y2 + h2 and
		x2 < x1 + w1 and
		y2 < y1 + h1;
}

void Entity::setRandPos(){
	float x = rand() % 750;
	float y = rand() % 550;
	setX(x);
	setY(y);
}
