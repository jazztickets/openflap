#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <cstdlib>
#include <iostream>
#include <iomanip>
#include <stdexcept>
#include <sstream>
#include <list>

class _Player;
class _Wall;

void InitGame();
void Died();
void SpawnWall(float MidY);
void CheckCollision();
void Update(float FrameTime);
void Render(float Blend);
void DeleteObjects();
void DrawText(const std::string &Text, int X, int Y, const SDL_Color &Color);
bool CheckWallCollision(_Wall *Wall); 

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
const float FPS = 60.0;
const float JUMP_POWER = 11.0f;
const float GRAVITY = 0.5f;
const float MAX_FALL_SPEED = 15.5f;
const float WALL_VELOCITY = -3.5f;
const float WALL_WIDTH = 100.0f;
const float SPACING = 105.0f;
const float SPAWNTIME = 1.6f;
const float WALL_BUFFER = 50.0f;
const SDL_Color ColorWhite = { 255, 255, 255, 255 };

static float HighScore = 0.0f;
static float Time;
static float SpawnTimer;
static _Player *Player;
static SDL_Renderer *Renderer = NULL;
static SDL_Texture *Texture = NULL;
static SDL_Texture *WallTexture = NULL;
static SDL_Texture *TextTexture = NULL;
static TTF_Font *Font = NULL;
std::list<_Wall *> Walls;
std::list<_Wall *>::iterator WallsIterator;

class _Player {

	public:

		_Player() : X(100), Y(0), VelocityX(0), VelocityY(0), Texture(NULL) {}
		~_Player() {
		}

		void Init(SDL_Texture *Texture) {
			Sprite.w = 64;
			Sprite.h = 64;
			LastX = X;
			LastY = Y;
			Radius = 29;
			this->Texture = Texture;
		}

		void Update(float FrameTime) {
			LastX = X;
			LastY = Y;
			X += VelocityX;
			Y += VelocityY;
			if(Y < 0.0f)
				Y = 0.0f;
			VelocityY += GRAVITY;
			if(VelocityY > MAX_FALL_SPEED)
				VelocityY = MAX_FALL_SPEED;

		}

		void Jump() {
			VelocityY = -JUMP_POWER;
		}

		void Render(float Blend) {
			Sprite.x = (Uint32)(X * Blend + LastX  * (1.0 - Blend)) - Radius;
			Sprite.y = (Uint32)(Y * Blend + LastY  * (1.0 - Blend)) - Radius;
	
			SDL_RenderCopy(Renderer, Texture, NULL, &Sprite);
		}
	
		float X, Y;
		float LastX, LastY;
		float VelocityX, VelocityY;
		float Radius;
		SDL_Rect Sprite;
		SDL_Texture *Texture;

};

class _Wall {

	public:

		_Wall() : X(SCREEN_WIDTH), Y(0), VelocityX(WALL_VELOCITY), VelocityY(0), Texture(NULL) {}
		~_Wall() {
		}

		void Init(SDL_Texture *Texture, float Y, float SizeX, float SizeY) {
			Size.w = 64;
			Size.h = 64;
			this->Y = Y;
			Sprite.w = SizeX;
			Sprite.h = SizeY;
			LastX = X;
			LastY = Y;
			this->Texture = Texture;
		}

		void Update(float FrameTime) {
			LastX = X;
			LastY = Y;
			X += VelocityX;
			Y += VelocityY;
		}

		void Render(float Blend) {
			Sprite.x = (Uint32)(X * Blend + LastX  * (1.0 - Blend));
			Sprite.y = (Uint32)(Y * Blend + LastY  * (1.0 - Blend));
	
			SDL_RenderCopy(Renderer, Texture, NULL, &Sprite);
		}
	
		float X, Y;
		float VelocityX, VelocityY;
		float LastX, LastY;
		float Radius;
		SDL_Rect Size;
		SDL_Rect Sprite;
		SDL_Texture *Texture;

};

int main() {

	if(SDL_Init(SDL_INIT_EVERYTHING) == -1) {
		std::cout << SDL_GetError() << std::endl;
		return 1;
	}
	
	if(TTF_Init() != 0) {
		std::cout << SDL_GetError() << std::endl;
		return 1;
	}

	SDL_Window *Window = NULL;
	Window = SDL_CreateWindow("openflap", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	if(!Window) {
		std::cout << SDL_GetError() << std::endl;
		return 1;
	}
	
	//Renderer = SDL_CreateRenderer(Window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	Renderer = SDL_CreateRenderer(Window, -1, 0);
	if(Renderer == NULL) {
		std::cout << SDL_GetError() << std::endl;
		return 1;
	}
	
	Font = TTF_OpenFont("arimo_regular.ttf", 18);
	if(Font == NULL) {
		std::cout << SDL_GetError() << std::endl;
		return 1;
	}

	Texture = IMG_LoadTexture(Renderer, "player.png");
	if(Texture == NULL) {
		std::cout << SDL_GetError() << std::endl;
		return 1;
	}

	WallTexture = IMG_LoadTexture(Renderer, "wall.png");
	if(WallTexture == NULL) {
		std::cout << SDL_GetError() << std::endl;
		return 1;
	}

	InitGame();
	
	bool Quit = false;
	float Timer = SDL_GetPerformanceCounter();
	float TimeStep = 1.0f / FPS;
	float TimeStepAccumulator = 0.0f;
    while(!Quit) {
		
		// Get frametime
		float FrameTime = (SDL_GetPerformanceCounter() - Timer) / (float)SDL_GetPerformanceFrequency();
		Timer = SDL_GetPerformanceCounter();
		
		SDL_PumpEvents();
		SDL_Event Event;
		while(SDL_PollEvent(&Event)) {
			if(Event.type == SDL_QUIT)
				Quit = true;
			if(Event.type == SDL_KEYDOWN) {
				if(Event.key.keysym.sym == SDLK_ESCAPE)
					Quit = true;
				else if(Event.key.repeat == 0 && Event.key.keysym.sym == SDLK_SPACE)
					Player->Jump();
				/*
				else if(Event.key.repeat == 0 && Event.key.keysym.sym == SDLK_RIGHT)
					Player->VelocityX = 1;
				else if(Event.key.repeat == 0 && Event.key.keysym.sym == SDLK_LEFT)
					Player->VelocityX = -1;
				else if(Event.key.repeat == 0 && Event.key.keysym.sym == SDLK_UP)
					Player->VelocityY = -1;
				else if(Event.key.repeat == 0 && Event.key.keysym.sym == SDLK_DOWN)
					Player->VelocityY = 1;
				else {

					Player->VelocityX = 0;
					Player->VelocityY = 0;
				}
				*/

			}
		}
		
		// Game loop
		TimeStepAccumulator += FrameTime;
		if(TimeStepAccumulator > 3.0f)
			TimeStepAccumulator = 3.0f;
		
		while(TimeStepAccumulator >= TimeStep) {
			Update(TimeStep);
			TimeStepAccumulator -= TimeStep;
		}

		Render(TimeStepAccumulator * FPS);

		float ExtraTime = 1.0f / FPS - FrameTime;
		if(ExtraTime > 0.0f) {
			SDL_Delay((Uint32)(ExtraTime * 1000));
		}
		Time += FrameTime;
	}
		
	DeleteObjects();
	SDL_DestroyTexture(Texture);
	SDL_DestroyTexture(WallTexture);
	TTF_CloseFont(Font);
	SDL_DestroyRenderer(Renderer);
	SDL_DestroyWindow(Window);
	SDL_Quit();
	
	return 0;
}

void Died() {

	if(Time > HighScore) {
		HighScore = Time;
	}

	InitGame();
}

void InitGame() {
	TextTexture = NULL;
	DeleteObjects();
	Player = new _Player();
	
	Player->Init(Texture);
	SpawnTimer = 0.0f;
	Time = 0.0f;
}

void CheckCollision() {
	if(Player->Y > SCREEN_HEIGHT)
		Died();

	for(WallsIterator = Walls.begin(); WallsIterator != Walls.end(); ++WallsIterator) {
		_Wall *Wall = *WallsIterator;
		if(CheckWallCollision(Wall)) {
			Died();
		}
	}
}

void Update(float FrameTime) {
	Player->Update(FrameTime);
	for(WallsIterator = Walls.begin(); WallsIterator != Walls.end(); ) {
		_Wall *Wall = *WallsIterator;
		Wall->Update(FrameTime);
		if(Wall->X + Wall->Sprite.w < 0) {
			delete Wall;
			WallsIterator = Walls.erase(WallsIterator);
		}
		else {
			++WallsIterator;
		}
	}

	CheckCollision();

	SpawnTimer -= FrameTime;
	if(SpawnTimer <= 0.0f) {
		SpawnWall((rand() % int(SCREEN_HEIGHT - (SPACING+WALL_BUFFER)*2)) + WALL_BUFFER + SPACING);
		SpawnTimer = SPAWNTIME;
	}
}

void Render(float Blend) {
	SDL_RenderClear(Renderer);
	for(WallsIterator = Walls.begin(); WallsIterator != Walls.end(); ++WallsIterator) {
		(*WallsIterator)->Render(Blend);
	}
	Player->Render(Blend);

	std::ostringstream Buffer;
	Buffer << std::fixed << std::setprecision(2) << "Time: " << Time;
	DrawText(Buffer.str(), SCREEN_WIDTH - 160, 15, ColorWhite);

	Buffer.str("");
	Buffer << std::fixed << std::setprecision(2) << "High Score: " << HighScore;
	DrawText(Buffer.str(), SCREEN_WIDTH - 160, 35, ColorWhite);
	SDL_RenderPresent(Renderer);
}

void SpawnWall(float MidY) {
	float StartY, EndY;
	StartY = 0;
	EndY = MidY - SPACING;
	_Wall *WallTop = new _Wall();
	WallTop->Init(WallTexture, StartY, WALL_WIDTH, EndY - StartY);
	Walls.push_back(WallTop);

	StartY = MidY + SPACING;
	EndY = SCREEN_HEIGHT;
	_Wall *WallBottom = new _Wall();
	WallBottom->Init(WallTexture, StartY, WALL_WIDTH, EndY - StartY);
	Walls.push_back(WallBottom);
}

bool CheckWallCollision(_Wall *Wall) {
	float AABB[4] = { Wall->X, Wall->Y, Wall->X + Wall->Sprite.w, Wall->Y + Wall->Sprite.h };

	// Get closest point on AABB
	float X = Player->X;
	float Y = Player->Y;
	if(X < AABB[0]) {
		X = AABB[0];
	}
	if(Y < AABB[1]) {
		Y = AABB[1];
	}
	if(X > AABB[2]) {
		X = AABB[2];
	}
	if(Y > AABB[3]) {
		Y = AABB[3];
	}

	// Test circle collision with point
	float DistanceX = X - Player->X;
	float DistanceY = Y - Player->Y;
	float DistanceSquared = (DistanceX * DistanceX + DistanceY * DistanceY);
	bool Hit = DistanceSquared < Player->Radius * Player->Radius;

	return Hit;
}

void DeleteObjects() {
	delete Player;
	for(WallsIterator = Walls.begin(); WallsIterator != Walls.end(); ++WallsIterator) {
		delete (*WallsIterator);
	}

	Walls.clear();
}

void DrawText(const std::string &Text, int X, int Y, const SDL_Color &Color) {
	SDL_Rect Bounds;
	Bounds.x = X;
	Bounds.y = Y;
	if(TextTexture)
		SDL_DestroyTexture(TextTexture);
	SDL_Surface *TextSurface = TTF_RenderText_Blended(Font, Text.c_str(), Color);
	TextTexture = SDL_CreateTextureFromSurface(Renderer, TextSurface);
	SDL_QueryTexture(TextTexture, NULL, NULL, &Bounds.w, &Bounds.h);
	SDL_RenderCopy(Renderer, TextTexture, NULL, &Bounds);
	SDL_FreeSurface(TextSurface);
}
