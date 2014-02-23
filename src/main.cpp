#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include <iostream>
#include <iomanip>
#include <stdexcept>
#include <sstream>
#include <list>
#include <random.h>
#include <vector2.h>
#include <physics.h>
#include <player.h>
#include <sprite.h>
#include <config.h>
#include <constants.h>

enum GameState {
	STATE_PLAY,
	STATE_DIED,
};

void InitGame();
void Died();
void SpawnWall(float MidY);
void CheckCollision();
void Update(float FrameTime);
void Render(float Blend);
void DeleteObjects();
void DrawText(const std::string &Text, int X, int Y, const SDL_Color &Color);
bool CheckWallCollision(_Sprite *Wall); 

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
static SDL_Joystick *Joystick = NULL;
static Mix_Chunk *DieSound = NULL;
static Mix_Chunk *SwoopSound = NULL;
static Mix_Music *Music = NULL;
static std::string Songs[2] = { "song_crunch.ogg", "song_jazztown.ogg" };
std::list<_Sprite *> Walls;
std::list<_Sprite *> Backgrounds;
typedef std::list<_Sprite *>::iterator SpriteIteratorType;

int main() {
	
	// Init config system
	Config.Init("settings.cfg");
	
	if(SDL_Init(SDL_INIT_EVERYTHING) == -1) {
		std::cout << SDL_GetError() << std::endl;
		return 1;
	}

	Random.SetSeed(SDL_GetPerformanceCounter());

	if(Config.AudioEnabled) {
		int MixFlags = MIX_INIT_OGG;
		int MixInit = Mix_Init(MixFlags);
		if(MixInit & MixFlags != MixFlags) {
			std::cout << Mix_GetError() << std::endl;
			return 1;
		}
		
		if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024) < 0) {
			std::cout << Mix_GetError() << std::endl;
			return 1;
		}
		
		SwoopSound = Mix_LoadWAV("swoop.ogg");
		DieSound = Mix_LoadWAV("die.ogg");
		Music = Mix_LoadMUS(Songs[Random.GenerateRange(0, 1)].c_str());
		Mix_Volume(-1, Config.SoundVolume * MIX_MAX_VOLUME);
	}
	
	if(TTF_Init() != 0) {
		std::cout << SDL_GetError() << std::endl;
		return 1;
	}

	// Set up window
	SDL_Window *Window = NULL;
	Uint32 Flags = SDL_WINDOW_SHOWN;
	if(Config.Fullscreen)
		Flags |= SDL_WINDOW_FULLSCREEN | SDL_WINDOW_INPUT_GRABBED;
	Window = SDL_CreateWindow("openflap", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, Config.ScreenWidth, Config.ScreenHeight, Flags);
	if(!Window) {
		std::cout << SDL_GetError() << std::endl;
		return 1;
	}
	
	// Set up renderer
	Flags = 0;
	Flags |= SDL_RENDERER_ACCELERATED;
	if(Config.Vsync)
		Flags |= SDL_RENDERER_PRESENTVSYNC;
	Renderer = SDL_CreateRenderer(Window, -1, Flags);
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
		Joystick = SDL_JoystickOpen(0);
	
	InitGame();
	
	if(Mix_PlayMusic(Music, -1) == -1) {
		std::cout << SDL_GetError() << std::endl;
		return 1;
	}
	Mix_VolumeMusic(Config.MusicVolume * MIX_MAX_VOLUME);
		
	bool Quit = false;
	float Timer = SDL_GetPerformanceCounter();
	float TimeStep = GAME_TIMESTEP;
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
					if(Config.AudioEnabled)
						Mix_PlayChannel(-1, SwoopSound, 0);
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

		Render(TimeStepAccumulator * GAME_FPS);

		float ExtraTime = 1.0f / GAME_FPS - FrameTime;
		if(ExtraTime > 0.0f) {
			//SDL_Delay((Uint32)(ExtraTime * 1000));
		}
		
		//std::cout << FrameTime << std::endl;
	}
		
	DeleteObjects();
	SDL_DestroyTexture(Texture);
	SDL_DestroyTexture(WallTexture);
	TTF_CloseFont(Font);
	SDL_JoystickClose(Joystick);
	SDL_DestroyRenderer(Renderer);
	SDL_DestroyWindow(Window);
	if(Config.AudioEnabled) {
		Mix_FreeChunk(DieSound);
		Mix_FreeChunk(SwoopSound);
		Mix_FreeMusic(Music);
		Mix_CloseAudio();
		Mix_Quit();
	}
	SDL_Quit();
	
	Config.Close();
	
	return 0;
}

void Died() {
	Mix_PlayChannel(-1, DieSound, 0);
	
	if(Time > HighScore) {
		HighScore = Time;
	}
	
	for(SpriteIteratorType WallsIterator = Walls.begin(); WallsIterator != Walls.end(); ++WallsIterator) {
		(*WallsIterator)->Physics.SetVelocity(Vector2(0, 0));
		(*WallsIterator)->Update(0);
	}

	State = STATE_DIED;
	DiedTimer = DIED_WAIT_TIME;
}

void InitGame() {
	State = STATE_PLAY;
	TextTexture = NULL;
	DeleteObjects();
	Player = new _Player(_Physics(Vector2(100, 0), Vector2(0, 0), Vector2(0, GRAVITY)));
	Player->Init(Texture);
	SpawnTimer = 0.0f;
	Time = 0.0f;
	
	_Sprite *Background;
	Background = new _Sprite();
	Background->Texture = BackTexture[0];
	Background->Bounds.w = Config.ScreenWidth;
	Background->Bounds.h = Config.ScreenHeight;
	Background->Physics = _Physics(Vector2(0, 0), Vector2(-5, 0), Vector2(0, 0));
	Backgrounds.push_back(Background);
	
	Background = new _Sprite();
	Background->Texture = BackTexture[0];
	Background->Bounds.w = Config.ScreenWidth;
	Background->Bounds.h = Config.ScreenHeight;
	Background->Physics = _Physics(Vector2(Config.ScreenWidth, 0), Vector2(-5, 0), Vector2(0, 0));
	Backgrounds.push_back(Background);
	
	Background = new _Sprite();
	Background->Texture = BackTexture[1];
	Background->Bounds.w = Config.ScreenWidth;
	Background->Bounds.h = 200;
	Background->Physics = _Physics(Vector2(0, Config.ScreenHeight - Background->Bounds.h), Vector2(-30, 0), Vector2(0, 0));
	Backgrounds.push_back(Background);
	
	Background = new _Sprite();
	Background->Texture = BackTexture[1];
	Background->Bounds.w = Config.ScreenWidth;
	Background->Bounds.h = 200;
	Background->Physics = _Physics(Vector2(Config.ScreenWidth, Config.ScreenHeight - Background->Bounds.h), Vector2(-30, 0), Vector2(0, 0));
	Backgrounds.push_back(Background);
	
	Background = new _Sprite();
	Background->Texture = BackTexture[1];
	Background->Bounds.w = Config.ScreenWidth;
	Background->Bounds.h = 100;
	Background->Physics = _Physics(Vector2(0, Config.ScreenHeight - Background->Bounds.h), Vector2(-50, 0), Vector2(0, 0));
	Backgrounds.push_back(Background);
	
	Background = new _Sprite();
	Background->Texture = BackTexture[1];
	Background->Bounds.w = Config.ScreenWidth;
	Background->Bounds.h = 100;
	Background->Physics = _Physics(Vector2(Config.ScreenWidth, Config.ScreenHeight - Background->Bounds.h), Vector2(-50, 0), Vector2(0, 0));
	Backgrounds.push_back(Background);
}

void CheckCollision() {
	if(Player->Physics.GetPosition().Y > Config.ScreenHeight + Player->Radius)
		Died();

	for(SpriteIteratorType WallsIterator = Walls.begin(); WallsIterator != Walls.end(); ++WallsIterator) {
		_Sprite *Wall = *WallsIterator;
		if(CheckWallCollision(Wall)) {
			Died();
		}
	}
}

void Update(float FrameTime) {
	if(State == STATE_PLAY)
		Time += FrameTime;
		
	Player->Update(FrameTime);
	for(SpriteIteratorType WallsIterator = Walls.begin(); WallsIterator != Walls.end(); ) {
		_Sprite *Wall = *WallsIterator;
		Wall->Update(FrameTime);
		if(Wall->Physics.GetPosition().X + Wall->Bounds.w < 0) {
			delete Wall;
			WallsIterator = Walls.erase(WallsIterator);
		}
		else {
			++WallsIterator;
		}
	}
	
	for(SpriteIteratorType BackgroundIterator = Backgrounds.begin(); BackgroundIterator != Backgrounds.end(); ) {
		_Sprite *Sprite = *BackgroundIterator;
		Sprite->Update(FrameTime);
		if(Sprite->Physics.GetPosition().X <= -Config.ScreenWidth) {
			Sprite->Physics.SetPosition(Vector2(Config.ScreenWidth, Sprite->Physics.GetPosition().Y));
			Sprite->Update(0);
		}
		
		++BackgroundIterator;
	}
	

	if(State == STATE_PLAY) {
		
		SpawnTimer -= FrameTime;
		if(SpawnTimer <= 0.0f) {
			SpawnWall(Random.GenerateRange(Config.ScreenHeight/2 - SPAWN_RANGE, Config.ScreenHeight/2 + SPAWN_RANGE));
			SpawnTimer = SPAWNTIME;
		}
		
		CheckCollision();
	}
	else
		DiedTimer -= FrameTime;
}

void Render(float Blend) {
	SDL_RenderClear(Renderer);
	
	for(SpriteIteratorType BackgroundIterator = Backgrounds.begin(); BackgroundIterator != Backgrounds.end(); ++BackgroundIterator) {
		(*BackgroundIterator)->Render(Renderer, Blend);
	}

	for(SpriteIteratorType WallsIterator = Walls.begin(); WallsIterator != Walls.end(); ++WallsIterator) {
		(*WallsIterator)->Render(Renderer, Blend);
	}
	Player->Render(Renderer, Blend);

	std::ostringstream Buffer;
	Buffer << std::fixed << std::setprecision(2) << "Time: " << Time;
	DrawText(Buffer.str(), Config.ScreenWidth - 160, 15, ColorWhite);

	Buffer.str("");
	Buffer << std::fixed << std::setprecision(2) << "High Score: " << HighScore;
	DrawText(Buffer.str(), Config.ScreenWidth - 160, 35, ColorWhite);

	if(State == STATE_DIED)
		DrawText("You Died!", 10, 10, ColorRed);

	SDL_RenderPresent(Renderer);
}

void SpawnWall(float MidY) {
	float StartY, EndY;
	StartY = 0;
	EndY = MidY - SPACING;
	_Sprite *WallTop = new _Sprite();
	WallTop->Texture = WallTexture;
	WallTop->Physics = _Physics(Vector2(Config.ScreenWidth, StartY), Vector2(WALL_VELOCITY, 0), Vector2(0, 0));
	WallTop->Bounds.w = WALL_WIDTH;
	WallTop->Bounds.h = EndY - StartY;
	Walls.push_back(WallTop);

	StartY = MidY + SPACING;
	EndY = Config.ScreenHeight;
	_Sprite *WallBottom = new _Sprite();
	WallBottom->Texture = WallTexture;
	WallBottom->Physics = _Physics(Vector2(Config.ScreenWidth, StartY), Vector2(WALL_VELOCITY, 0), Vector2(0, 0));
	WallBottom->Bounds.w = WALL_WIDTH;
	WallBottom->Bounds.h = EndY - StartY;
	Walls.push_back(WallBottom);
}

bool CheckWallCollision(_Sprite *Wall) {
	float AABB[4] = { Wall->Physics.GetPosition().X, Wall->Physics.GetPosition().Y, Wall->Physics.GetPosition().X + Wall->Bounds.w, Wall->Physics.GetPosition().Y + Wall->Bounds.h };

	// Get closest point on AABB
	float X = Player->Physics.GetPosition().X;
	float Y = Player->Physics.GetPosition().Y;
	if(X < AABB[0])
		X = AABB[0];
	if(Y < AABB[1])
		Y = AABB[1];
	if(X > AABB[2])
		X = AABB[2];
	if(Y > AABB[3])
		Y = AABB[3];

	// Test circle collision with point
	float DistanceX = X - Player->Physics.GetPosition().X;
	float DistanceY = Y - Player->Physics.GetPosition().Y;
	float DistanceSquared = (DistanceX * DistanceX + DistanceY * DistanceY);
	bool Hit = DistanceSquared < Player->Radius * Player->Radius;

	return Hit;
}

void DeleteObjects() {
	
	delete Player;
	for(SpriteIteratorType WallsIterator = Walls.begin(); WallsIterator != Walls.end(); ++WallsIterator) {
		delete (*WallsIterator);
	}
	for(SpriteIteratorType BackgroundIterator = Backgrounds.begin(); BackgroundIterator != Backgrounds.end(); ++BackgroundIterator) {
		delete (*BackgroundIterator);
	}
	
	Backgrounds.clear();
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
