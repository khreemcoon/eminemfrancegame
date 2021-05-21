#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "player.h"
#include "entity.h"


Player::Player(float pX, float pY, SDL_Texture* pTex)
	:Entity{pX, pY, pTex}
{
	setX(pX);
	setY(pY);
}

void Player::Update(){
	setX(getX() + xVel);
	setY(getY() + yVel);
}

const char* Player::getScoreChar(){
	std::string s =std::to_string(score);
	return s.c_str();
}

void Player::setHS(){
	if(score >= highscore) highscore = score;
	else highscore = highscore;
}
const char* Player::getHSChar(){
	std::string s =std::to_string(highscore);
	return s.c_str();
}