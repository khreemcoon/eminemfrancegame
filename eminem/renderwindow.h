#pragma once

#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include "entity.h"

class RenderWin{
private:
	SDL_Window* window;
	SDL_Renderer* renderer;
public:
	RenderWin();
	SDL_Texture* loadTexture(const char* path);
	void Create(const char* pTitle, int pW, int pH);
	void Render(Entity &entity);
	void Render(SDL_Texture* pTex, float pX, float pY); 
	void Render(const char* pText, TTF_Font* pFont, int pX, int pY, SDL_Color pColor);
	void RenderFS(SDL_Texture* pTex);
	void Clear();
	void Display();
	void Clean();
};
