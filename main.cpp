#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>


#include "renderwindow.h"
#include "entity.h"
#include "player.h"
#include "timer.h"

//THE RULER.
RenderWin window;

//game variables
int GAMESTATE     = 0;
int difficulty    = 0;
int scoreReq      = 0;
int timeRemaining = 0;
std::string timeRem = std::to_string(timeRemaining);
const char* timeStr = timeRem.c_str();
bool gameRunning  = false;
double dt         = 0;


//w/h
const int WIDTH  = 800;
const int HEIGHT = 600;

//fonts
TTF_Font* font;
TTF_Font* fontMM;

//textures
//entity textures
SDL_Texture* playerTexture    = NULL;
SDL_Texture* enemyTex         = NULL;
//menu textures
SDL_Texture* mmTex            = NULL;
SDL_Texture* difselTex        = NULL;
SDL_Texture* splashTex        = NULL;
SDL_Texture* winTex           = NULL;
SDL_Texture* lossTex          = NULL;
//button textures
SDL_Texture* pBtnTex          = NULL;
SDL_Texture* qBtnTex          = NULL;
//diffilculty textures
SDL_Texture* easy             = NULL;
SDL_Texture* med              = NULL;
SDL_Texture* hard             = NULL;

//Colors
SDL_Color black   = {0,0,0};
SDL_Color red     = {255,0,0};
SDL_Color green   = {0,255,0};
SDL_Color blue    = {0,0,255};
SDL_Color white   = {255,255,255};
SDL_Color yellow  = {255,255,0};

//Audio
//Music
Mix_Music* menuMusic = NULL;
Mix_Music* mainMusic = NULL;
//SFX
Mix_Chunk* intro = NULL;
Mix_Chunk* collect = NULL;
Mix_Chunk* victory = NULL;
Mix_Chunk* loss = NULL;
Mix_Chunk* click = NULL;



bool Init(){
	//makes so game goes
	gameRunning = true;
	GAMESTATE = -1;
	
	//initializes everything that I need
	if(SDL_Init(SDL_INIT_EVERYTHING) < 0) printf("SDL failed initializing! SDL_Error: %s\n", SDL_GetError());
	if(IMG_Init(IMG_INIT_PNG) < 0) printf("IMG failed initializing! IMG_Error: %s\n", SDL_GetError());
	if(TTF_Init() < 0)printf("TTF could not initialize! TTF_Error: %s\n", TTF_GetError);
	if(Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096) < 0) printf("Mixer could not initialize!");

	//make the window
	window.Create("Eminem Hates France", WIDTH, HEIGHT);
	
	//load the textures that I need
	//entity textures
	playerTexture   = window.loadTexture("images/player/emnem.png");
	enemyTex        = window.loadTexture("images/enemies/french.png");
	//menu textures
	mmTex           = window.loadTexture("images/main menu images/mainmenu.png");
	difselTex       = window.loadTexture("images/main menu images/difSelect.png");
	splashTex       = window.loadTexture("images/main menu images/splashscreen.png");
	winTex          = window.loadTexture("images/main menu images/victory.png");
	lossTex         = window.loadTexture("images/main menu images/loss.png");
	//button textures
	pBtnTex         = window.loadTexture("images/main menu images/btnPlay.png");
	qBtnTex         = window.loadTexture("images/main menu images/btnQuit.png");
	//difficulty textures
	easy            = window.loadTexture("images/main menu images/easy.png");
	med             = window.loadTexture("images/main menu images/med.png");
	hard            = window.loadTexture("images/main menu images/hard.png");

	//loads the font and checks for errors in loading the font
	font = TTF_OpenFont("fonts/Panic.ttf", 32);
	fontMM = TTF_OpenFont("fonts/LibreBaskerville-Regular.ttf", 48);
	if(font == NULL) printf("Font error! TTF_Error: %s\n", TTF_GetError());
	if(fontMM == NULL) printf("Font error! TTF_Error: %s\n", TTF_GetError());

	//Load audio
	intro     = Mix_LoadWAV("sfx/intro.wav");
	collect   = Mix_LoadWAV("sfx/french.wav");
	click     = Mix_LoadWAV("sfx/click.wav");
	victory   = Mix_LoadWAV("sfx/victory.wav");
	loss      = Mix_LoadWAV("sfx/loss.wav");

	//yayy
	return true;
}
//actually loads the game
bool load = Init();

//create player and enemies and shit
//player
Player player(600, 300, playerTexture);
Entity french1(0,0,enemyTex);
//buttons
Entity playButton(640, 20, pBtnTex);
Entity quitButton(640, 80, qBtnTex);
//difficulty
Entity easyBtn(640, 90, easy);
Entity medBtn(640, 180, med);
Entity hardBtn(640, 270,hard);

//Timers
Timer splashTimer(3200, true);
Timer countdown(1000, false);

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
							if(playButton.Collision(x,y,3,3,playButton.getX(),playButton.getY(),playButton.getW(),playButton.getH())){GAMESTATE = 4; Mix_PlayChannel(-1, click, 0);}
							if(quitButton.Collision(x,y,3,3,quitButton.getX(),quitButton.getY(),quitButton.getW(),quitButton.getH())){gameRunning = false; Mix_PlayChannel(-1, click, 0);}
						}

					break;
				}
			break;
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
			case 2:
				switch (e.type){
					case SDL_KEYDOWN:
						switch(e.key.keysym.sym){
							case SDLK_ESCAPE:
								GAMESTATE = 0;
								break;
							}
					}
			break;
			case 3:
				switch (e.type){
					case SDL_KEYDOWN:
						switch(e.key.keysym.sym){
							case SDLK_ESCAPE:
								GAMESTATE = 0;
								break;
							}
					}
			break;
			case 4:
				switch(e.type){
					case SDL_MOUSEBUTTONDOWN:
						if(SDL_BUTTON_LEFT){
							int x, y;
							SDL_GetMouseState(&x,&y);
							if(easyBtn.Collision(x,y,3,3,easyBtn.getX(),easyBtn.getY(),easyBtn.getW(),easyBtn.getH())){
								Mix_PlayChannel(-1, click, 0);
								difficulty = 0;
								GAMESTATE = 1;
								switch(difficulty){
									case 0:
										scoreReq = 50;
										timeRemaining = 90;
									break;				
									case 1:
										timeRemaining = 60;
										scoreReq = 65;
									break;
									case 2:
										timeRemaining = 45;
										scoreReq = 80;
									break;
								}
							}
							if(medBtn.Collision(x,y,3,3,medBtn.getX(),medBtn.getY(),medBtn.getW(),medBtn.getH())){
								Mix_PlayChannel(-1, click, 0);
								difficulty = 1;
								GAMESTATE = 1;
								switch(difficulty){
									case 0:
										scoreReq = 50;
										timeRemaining = 90;
									break;				
									case 1:
										timeRemaining = 60;
										scoreReq = 65;
									break;
									case 2:
										timeRemaining = 45;
										scoreReq = 80;
									break;
								}
							}
							if(hardBtn.Collision(x,y,3,3,hardBtn.getX(),hardBtn.getY(),hardBtn.getW(),hardBtn.getH())){
								Mix_PlayChannel(-1, click, 0);
								difficulty = 2;
								GAMESTATE = 1;
								switch(difficulty){
									case 0:
										scoreReq = 50;
										timeRemaining = 90;
									break;				
									case 1:
										timeRemaining = 60;
										scoreReq = 65;
									break;
									case 2:
										timeRemaining = 45;
										scoreReq = 80;
									break;
								}
							}
						}
				}
			break;
		}
	}
}

bool tick;
//main game loop
void gLoop(){
	//refreshes window
	window.Clear();

	//GAMESTATES
	
	if(GAMESTATE == -1){
		window.RenderFS(splashTex);
		splashTimer.Update();
		if(splashTimer.getShot())GAMESTATE = 0;
	}
	if(GAMESTATE == 0){
		//render shit needed
		window.RenderFS(mmTex);
		window.Render("Marshall Hates France", fontMM, 20, 500, white);
		window.Render(playButton);
		window.Render(quitButton);
	}
	if(GAMESTATE == 1){
		if(player.getScore() >= scoreReq){ GAMESTATE = 2; Mix_PlayChannel(-1, victory, 0);}
		//render el frenchie,,,,
		window.Render(french1);
		//checks for collision between french and player
		if(player.Collision(french1.getX(), french1.getY(), french1.getW(), french1.getH(), player.getX(), player.getY(), player.getW(), player.getH())){
				Mix_PlayChannel(-1, collect, 0);
				player.addScore(1);
				french1.setRandPos();
		}
		//render eminem and call his update function
		window.Render(player);
		player.Update();
		//render player score
		window.Render(player.getScoreChar(), font, 30, 30, green);
		
		//time
		countdown.Update();
		if(timeRemaining > 0){
			if(countdown.getShot()){timeRemaining -=1; countdown.noShot();}
		}else if(timeRemaining <= 0) {Mix_PlayChannel(-1, loss, 0);GAMESTATE = 3;}
		
		timeRem = std::to_string(timeRemaining);
		timeStr = timeRem.c_str();
		window.Render(timeStr, font, WIDTH/2 -50, HEIGHT-590, black);
	}
	if(GAMESTATE == 2){
		window.RenderFS(winTex);
		window.Render("(press esc to go to main menu)", fontMM, 30, 400, black);
	}
	if(GAMESTATE == 3){
		window.RenderFS(lossTex);
		window.Render("(press esc to go to main menu)", fontMM, 30, 400, black);
	}
	if(GAMESTATE == 4){
		window.RenderFS(difselTex);
		window.Render("Difficulty:", fontMM, 540, 0, black);
		window.Render(easyBtn);
		window.Render("Easy:", fontMM, 510, 85, green);
		window.Render(medBtn);
		window.Render("Medium:", fontMM, 420, 180, yellow);
		window.Render(hardBtn);
		window.Render("Hard:", fontMM, 500, 270, red);
	}
	//actually make the window show everything
	window.Display();
	
}


int main(int argc, char ** args){
	//fps variables for making the fps usage not be 60%
	const int FPS = 60;
	const int frameDelay = 1000/FPS;
	Uint32 frameStart;
	int frameTime;
	//delta time
	Uint64 NOW = SDL_GetPerformanceCounter();
	Uint64 LAST = 0;
	
	Mix_PlayChannel(-1, intro, 0);
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
	Mix_CloseAudio();
	TTF_Quit();
	SDL_Quit();
	return 0;
}
