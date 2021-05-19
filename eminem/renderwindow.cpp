#include <SDL2/SDL_image.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "renderwindow.h"
#include "entity.h"


RenderWin::RenderWin(){

}


void RenderWin::Create(const char* pTitle, int pW, int pH){
	window = SDL_CreateWindow(pTitle, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, pW, pH, SDL_WINDOW_SHOWN);
	if(window == NULL) printf("Window failed to create. SDL_Error: %s\n", SDL_GetError());
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
}

SDL_Texture* RenderWin::loadTexture(const char* path){
	SDL_Texture* fTex = NULL;
	fTex = IMG_LoadTexture(renderer, path);
	if(fTex == NULL) printf("Could not load image at %s! IMG_Error: %s\n", path, IMG_GetError());
	return fTex;
}

void RenderWin::Clear(){
	SDL_SetRenderDrawColor(renderer, 255,255,255,255);
	SDL_RenderClear(renderer);
}

void RenderWin::Render(SDL_Texture* pTex, float pX, float pY){
	SDL_Rect rect;
	rect.x = pX;
	rect.y = pY;
	SDL_QueryTexture(pTex, NULL, NULL, &rect.w, &rect.h);
	SDL_RenderCopy(renderer, pTex, NULL, &rect);
}


void RenderWin::Render(Entity &entity){
	SDL_Rect rect;
	rect.x = entity.getX();
	rect.y = entity.getY();
	rect.w = entity.getW();
	rect.h = entity.getH();
	SDL_RenderCopy(renderer, entity.getTex(), NULL, &rect);
}

void RenderWin::Render(const char* pText, TTF_Font* pFont, int pX, int pY, SDL_Color pColor){

	SDL_Surface* surf;
	surf = TTF_RenderText_Solid(pFont, pText, pColor);
	SDL_Texture* tex = SDL_CreateTextureFromSurface(renderer, surf);

	SDL_Rect rect;
	rect.x = pX;
	rect.y = pY;
	rect.w = surf->w;
	rect.h = surf->h;

	SDL_RenderCopy(renderer, tex, NULL, &rect);
	SDL_FreeSurface(surf);
}

void RenderWin::RenderFS(SDL_Texture* pTex){
	SDL_RenderCopy(renderer, pTex, NULL, NULL);
}

void RenderWin::Display(){
	SDL_RenderPresent(renderer);
}

void RenderWin::Clean(){
	SDL_DestroyWindow(window);
}


