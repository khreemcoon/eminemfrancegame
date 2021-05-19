#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>


#include "renderwindow.h"
#include "entity.h"
#include "player.h"

//THE RULER.
RenderWin window;

//game variables
int GAMESTATE    = 0;
bool gameRunning = false;
double dt = 0;

//w/h
const int WIDTH  = 800;
const int HEIGHT = 600;

//fonts
TTF_Font* font;
TTF_Font* fontMM;

//textures
SDL_Texture* playerTexture    = NULL;
SDL_Texture* enemyTex         = NULL;
SDL_Texture* mmTex            = NULL;
SDL_Texture* pBtnTex          = NULL;
SDL_Texture* qBtnTex          = NULL;

//Colors
SDL_Color black   = {0,0,0};
SDL_Color red     = {255,0,0};
SDL_Color green   = {0,255,0};
SDL_Color blue    = {0,0,255};
SDL_Color white   = {255,255,255};

/*TODO*/
//1. Main menu, splash screen, lose screen, etc.
//2. The timer and losing
//3. Player Anims
//4. (maybe)enemy anims
//5. sfx
//6. the exporting

bool Init(){
	//makes so game goes
	gameRunning = true;
	
	//initializes everything that I need
	if(SDL_Init(SDL_INIT_EVERYTHING) < 0) printf("SDL failed initializing! SDL_Error: %s\n", SDL_GetError());
	if(IMG_Init(IMG_INIT_PNG) < 0) printf("IMG failed initializing! IMG_Error: %s\n", SDL_GetError());
	if(TTF_Init() < 0)printf("TTF could not initialize! TTF_Error: %s\n", TTF_GetError);
	
	//make the window
	window.Create("Eminem hates France", WIDTH, HEIGHT);
	
	//load the textures that I need
	playerTexture   = window.loadTexture("images/player/emnem.png");
	enemyTex        = window.loadTexture("images/enemies/french.png");
	mmTex           = window.loadTexture("images/main menu images/mainmenu.png");
	pBtnTex         = window.loadTexture("images/main menu images/btnPlay.png");
	qBtnTex         = window.loadTexture("images/main menu images/btnQuit.png");
	
	//loads the font and checks for errors in loading the font
	font = TTF_OpenFont("fonts/Panic.ttf", 32);
	fontMM = TTF_OpenFont("fonts/LibreBaskerville-Regular.ttf", 48);
	if(font == NULL) printf("Font error! TTF_Error: %s\n", TTF_GetError());
	if(fontMM == NULL) printf("Font error! TTF_Error: %s\n", TTF_GetError());

	//yayy
	return true;
}
//actually loads the game
bool load = Init();

//create player and enemies()
//TODO: put the enemy gen in a for loop for more enemies, also add them to a vector
Player player(600, 300, playerTexture);
Entity french1(30, 60, enemyTex);
Entity playButton(640, 20, pBtnTex);
Entity quitButton(640, 80, qBtnTex);

//------------------------------------
//CAUTION:
//HORRID CODE!!! LOOK AT OWN RISK!!!!!
//------------------------------------
void Input(){
	//the thing that we pull events from
	SDL_Event e;
	//-----------------------------
	//THE DIPSHITTERY OF SDL2 INPUT.
	//-----------------------------
	while(SDL_PollEvent(&e)){
		if(e.type == SDL_QUIT) gameRunning = false;
		switch(GAMESTATE){
			//--------------------------------
			//GAMESTATE 1 - THE MAIN GAMESTATE
			//--------------------------------
			case 1:
				switch (e.type){
					case SDL_KEYDOWN:
						switch(e.key.keysym.sym){
							case SDLK_ESCAPE:
								gameRunning = false;
								break;
							case SDLK_w: player.yVel = -(player.SPEED*dt); break;
							case SDLK_s: player.yVel = (player.SPEED*dt); break;
							case SDLK_d: player.xVel = (player.SPEED*dt); break;
							case SDLK_a: player.xVel = -(player.SPEED*dt); break;
						}
						break;
					case SDL_KEYUP:
						switch(e.key.keysym.sym){
							case SDLK_w: if(player.yVel <0) player.yVel = 0; break;
							case SDLK_s: if(player.yVel >0) player.yVel = 0; break;
							case SDLK_d: if(player.xVel >0) player.xVel = 0; break;
							case SDLK_a: if(player.xVel <0) player.xVel = 0; break;
						}
						break;
					}
			break;
			//-------------------------------------
			//GAMESTATE 0 - THE MAIN MENU GAMESTATE
			//-------------------------------------
			case 0:
				switch (e.type){
					case SDL_KEYDOWN:
						switch(e.key.keysym.sym){
							case SDLK_ESCAPE:
								gameRunning = false;
								break;
							case SDLK_SPACE:
								GAMESTATE = 1;
								break;
						}
					break;
					case SDL_MOUSEBUTTONDOWN:
						if(SDL_BUTTON_LEFT){
							int x, y;
							SDL_GetMouseState(&x, &y);
							if(playButton.Collision(x,y,3,3,playButton.getX(),playButton.getY(),playButton.getW(),playButton.getH()))GAMESTATE = 1;
							if(playButton.Collision(x,y,3,3,quitButton.getX(),quitButton.getY(),quitButton.getW(),quitButton.getH()))gameRunning = false;
						}

					break;
				}
			break;
		}
	}
}

//main game loop
void gLoop(){
	//refreshes window
	window.Clear();

	//GAMESTATES
	switch(GAMESTATE){
		case 0:
			//render shit needed
			window.RenderFS(mmTex);
			window.Render("Marshall Hates France", fontMM, 20, 500, white);
			window.Render(playButton);
			window.Render(quitButton);
			break;
		case 1:
			//render el frenchie,,,,
			window.Render(french1);
			//checks for collision between french and player
			if(player.Collision(french1.getX(), french1.getY(), french1.getW(), french1.getH(), player.getX(), player.getY(), player.getW(), player.getH())){
				player.addScore(1);
				french1.setRandPos();
			}
			//render eminem and call his update function
			window.Render(player);
			player.Update();

			//render player score
			window.Render(player.getScoreChar(), font, 30, 30, green);
			break;
	}

	//actually make the window show everything
	window.Display();
	
}


int main(int argc, char ** args){
	//fps variables for making the fps usage not be 60%
	const int FPS = 8;
	const int frameDelay = 1000/FPS;
	Uint32 frameStart;
	int frameTime;
	//delta time
	Uint64 NOW = SDL_GetPerformanceCounter();
	Uint64 LAST = 0;
	
	
	//the main game loop
	while(gameRunning){
		//more fps limiting shit
		frameStart = SDL_GetTicks();

		//dt
		LAST = NOW;
		NOW = SDL_GetPerformanceCounter();
		dt = (double)((NOW - LAST)*1000 / (double)SDL_GetPerformanceFrequency());

		//call the needed functions for mgl
		gLoop();
		Input();
		
		//calculate the fps and shite, then if fps is above then delay the game
		frameTime = SDL_GetTicks() - frameStart;
		if(frameDelay > frameTime)SDL_Delay(frameDelay - frameTime);
	}
	
	//cleanup
	window.Clean();
	TTF_CloseFont(font);
	TTF_CloseFont(fontMM);
	TTF_Quit();
	SDL_Quit();
	return 0;
}
