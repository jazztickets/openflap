#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>
#include <iomanip>
#include <stdexcept>
#include <sstream>
#include <list>
#include <random.h>
#include <vector2.h>
#include <physics.h>
#include <player.h>

enum GameState {
	STATE_PLAY,
	STATE_DIED,
};

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
const float FPS = 100.0;
const float JUMP_POWER = -670.0f;
const float GRAVITY = 1600.0f;
const float DIED_WAIT_TIME = 0.3f;
const float WALL_VELOCITY = -210.0f;
const float WALL_WIDTH = 100.0f;
const float SPACING = 105.0f;
const float SPAWNTIME = 1.6f;
const float WALL_BUFFER = 50.0f;
const SDL_Color ColorWhite = { 255, 255, 255, 255 };
const SDL_Color ColorRed = { 255, 0, 0, 255 };

static GameState State;
static float HighScore = 0.0f;
static float Time;
static float SpawnTimer;
static float DiedTimer = 0.0f;
static _Player *Player;
static SDL_Renderer *Renderer = NULL;
static SDL_Texture *Texture = NULL;
static SDL_Texture *WallTexture = NULL;
static SDL_Texture *TextTexture = NULL;
static SDL_Texture *BackTexture[4] = { NULL, NULL, NULL, NULL };
static TTF_Font *Font = NULL;
static SDL_Joystick *Controller = NULL;
std::list<_Wall *> Walls;
typedef std::list<_Wall *>::iterator WallsIteratorType;

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
			X += VelocityX * FrameTime;
			Y += VelocityY * FrameTime;
		}

		void Render(float Blend) {
			Sprite.x = (Uint32)(X * Blend + LastX * (1.0f - Blend) + 0.5f);
			Sprite.y = (Uint32)(Y * Blend + LastY * (1.0f - Blend) + 0.5f);
	
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
	
	Random.SetSeed(SDL_GetPerformanceCounter());
	
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
	
	Uint32 Flags = 0;
	Flags |= SDL_RENDERER_ACCELERATED;
	Flags |= SDL_RENDERER_PRESENTVSYNC;
	Renderer = SDL_CreateRenderer(Window, -1, Flags);
	if(Renderer == NULL) {
		std::cout << SDL_GetError() << std::endl;
		return 1;
	}
	
	SDL_GL_SetSwapInterval(true);
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
	
	BackTexture[0] = IMG_LoadTexture(Renderer, "back0.png");
	if(BackTexture[0] == NULL) {
		std::cout << SDL_GetError() << std::endl;
		return 1;
	}
	
	BackTexture[1] = IMG_LoadTexture(Renderer, "back1.png");
	if(BackTexture[1] == NULL) {
		std::cout << SDL_GetError() << std::endl;
		return 1;
	}
	
	if(SDL_NumJoysticks() > 0)
		Controller = SDL_JoystickOpen(0);
	
	InitGame();
	
	bool Quit = false;
	float Timer = SDL_GetPerformanceCounter();
	float TimeStep = 1.0f / FPS;
	float TimeStepAccumulator = 0.0f;
    while(!Quit) {
		
		// Get frametime
		float FrameTime = (SDL_GetPerformanceCounter() - Timer) / (float)SDL_GetPerformanceFrequency();
		Timer = SDL_GetPerformanceCounter();
		
		SDL_Event Event;
		while(SDL_PollEvent(&Event)) {
			bool Action = false;
			switch(Event.type) {
				case SDL_QUIT:
					Quit = true;
				break;
				case SDL_KEYDOWN:
					if(Event.key.repeat == 0) {
						if(Event.key.keysym.sym == SDLK_ESCAPE)
							Quit = true;
						else if(Event.key.keysym.sym == SDLK_SPACE)
							Action = true;
					}
				break;
				case SDL_JOYBUTTONDOWN:
				case SDL_MOUSEBUTTONDOWN:
					Action = true;
				break;
			}

			if(Action) {
				if(State == STATE_PLAY) {
					Player->Jump(JUMP_POWER);
				}
				else if(State == STATE_DIED && DiedTimer < 0) {
					InitGame();
				}
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
	}
		
	DeleteObjects();
	SDL_DestroyTexture(Texture);
	SDL_DestroyTexture(WallTexture);
	TTF_CloseFont(Font);
	SDL_JoystickClose(Controller);
	SDL_DestroyRenderer(Renderer);
	SDL_DestroyWindow(Window);
	SDL_Quit();
	
	return 0;
}

void Died() {

	if(Time > HighScore) {
		HighScore = Time;
	}
	for(WallsIteratorType WallsIterator = Walls.begin(); WallsIterator != Walls.end(); ++WallsIterator) {
		(*WallsIterator)->LastX = (*WallsIterator)->X;
	}

	State = STATE_DIED;
	DiedTimer = DIED_WAIT_TIME;
	Player->Physics.SetLastPosition(Player->Physics.GetPosition());
}

void InitGame() {
	State = STATE_PLAY;
	TextTexture = NULL;
	DeleteObjects();
	Player = new _Player(_Physics(Vector2(100, 0), Vector2(0, 0), Vector2(0, GRAVITY)));
	
	Player->Init(Texture);
	SpawnTimer = 0.0f;
	Time = 0.0f;
}

void CheckCollision() {
	if(Player->Physics.GetPosition().Y > SCREEN_HEIGHT)
		Died();

	for(WallsIteratorType WallsIterator = Walls.begin(); WallsIterator != Walls.end(); ++WallsIterator) {
		_Wall *Wall = *WallsIterator;
		if(CheckWallCollision(Wall)) {
			Died();
		}
	}
}

void Update(float FrameTime) {
	switch(State) {
		case STATE_PLAY: {
			Time += FrameTime;
			Player->Update(FrameTime);
			for(WallsIteratorType WallsIterator = Walls.begin(); WallsIterator != Walls.end(); ) {
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
				SpawnWall(Random.GenerateRange(WALL_BUFFER + SPACING, SCREEN_HEIGHT - (WALL_BUFFER + SPACING)));
				SpawnTimer = SPAWNTIME;
			}
		} break;
		case STATE_DIED:
			DiedTimer -= FrameTime;
		break;
	}
}

void Render(float Blend) {
	SDL_RenderClear(Renderer);
	
	SDL_Rect Background;
	Background.w = SCREEN_WIDTH;
	Background.h = SCREEN_HEIGHT;
	Background.x = int(-Time * 5) % SCREEN_WIDTH;
	Background.y = 0;
	SDL_RenderCopy(Renderer, BackTexture[0], NULL, &Background);
	
	Background.x = (int(-Time * 5) % SCREEN_WIDTH) + SCREEN_WIDTH;
	SDL_RenderCopy(Renderer, BackTexture[0], NULL, &Background);

	Background.w = SCREEN_WIDTH;
	Background.h = 200;
	Background.x = int(-Time * 30) % SCREEN_WIDTH;
	Background.y = SCREEN_HEIGHT - Background.h;
	SDL_RenderCopy(Renderer, BackTexture[1], NULL, &Background);

	Background.x = int(-Time * 30) % SCREEN_WIDTH + SCREEN_WIDTH;
	SDL_RenderCopy(Renderer, BackTexture[1], NULL, &Background);
	
	for(WallsIteratorType WallsIterator = Walls.begin(); WallsIterator != Walls.end(); ++WallsIterator) {
		(*WallsIterator)->Render(Blend);
	}
	Player->Render(Renderer, Blend);

	std::ostringstream Buffer;
	Buffer << std::fixed << std::setprecision(2) << "Time: " << Time;
	DrawText(Buffer.str(), SCREEN_WIDTH - 160, 15, ColorWhite);

	Buffer.str("");
	Buffer << std::fixed << std::setprecision(2) << "High Score: " << HighScore;
	DrawText(Buffer.str(), SCREEN_WIDTH - 160, 35, ColorWhite);

	if(State == STATE_DIED)
		DrawText("You Died!", 10, 10, ColorRed);

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
	float X = Player->Physics.GetPosition().X;
	float Y = Player->Physics.GetPosition().Y;
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
	float DistanceX = X - Player->Physics.GetPosition().X;
	float DistanceY = Y - Player->Physics.GetPosition().Y;
	float DistanceSquared = (DistanceX * DistanceX + DistanceY * DistanceY);
	bool Hit = DistanceSquared < Player->Radius * Player->Radius;

	return Hit;
}

void DeleteObjects() {
	delete Player;
	for(WallsIteratorType WallsIterator = Walls.begin(); WallsIterator != Walls.end(); ++WallsIterator) {
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
