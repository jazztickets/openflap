#include <SDL2/SDL.h>
#include <iostream>

void Update(double FrameTime);
void Render(double Blend);

const double FPS = 60.0;
static SDL_Texture *Texture = NULL;
static SDL_Renderer *Renderer = NULL;
static double x = 0, last_x = 0;

int main() {

	if(SDL_Init(SDL_INIT_EVERYTHING) == -1) {
		std::cout << SDL_GetError() << std::endl;
		return 1;
	}
	
	SDL_Window *Window = NULL;
	Window = SDL_CreateWindow("openflap", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 800, 600, SDL_WINDOW_SHOWN);
	if(!Window) {
		std::cout << SDL_GetError() << std::endl;
		return 1;
	}
	
	Renderer = SDL_CreateRenderer(Window, -1, 0);
	if(Renderer == NULL) {
		std::cout << SDL_GetError() << std::endl;
		return 1;
	}
	
	SDL_Surface *Image = NULL;
	Image = SDL_LoadBMP("test.bmp");
	if(Image == NULL) {
		std::cout << SDL_GetError() << std::endl;
		return 1;
	}

	Texture = SDL_CreateTextureFromSurface(Renderer, Image);
	SDL_FreeSurface(Image);
	
	bool Quit = false;
	double Timer = SDL_GetPerformanceCounter();
	double Time = 0.0;
	double TimeStep = 1.0 / FPS;
	double TimeStepAccumulator = 0.0;
    while(!Quit) {
		
		// Get frametime
		double FrameTime = (SDL_GetPerformanceCounter() - Timer) / (double)SDL_GetPerformanceFrequency();
		Timer = SDL_GetPerformanceCounter();
		
		SDL_PumpEvents();
		SDL_Event Event;
		while(SDL_PollEvent(&Event)) {
			if(Event.type == SDL_QUIT)
				Quit = true;
			if(Event.type == SDL_KEYDOWN)
				Quit = true;
		}
		
		// Game loop
		TimeStepAccumulator += FrameTime;
		if(TimeStepAccumulator > 3.0)
			TimeStepAccumulator = 3.0;
		
		while(TimeStepAccumulator >= TimeStep) {
			Update(TimeStep);
			TimeStepAccumulator -= TimeStep;
		}

		Render(TimeStepAccumulator * FPS);
		SDL_RenderPresent(Renderer);

		double ExtraTime = 1.0 / FPS - FrameTime;
		if(ExtraTime > 0.0) {
			SDL_Delay((Uint32)(ExtraTime * 1000));
		}
		Time += FrameTime;
		//std::cout << Time << " " << FrameTime << std::endl;
	}
		
	SDL_DestroyTexture(Texture);
	SDL_DestroyRenderer(Renderer);
	SDL_DestroyWindow(Window);
	SDL_Quit();
	
	return 0;
}

void Update(double FrameTime) {
	last_x = x;
	x += 100 * FrameTime;
}

void Render(double Blend) {
	//std::cout << Blend << std::endl;
	SDL_Rect pos;
	pos.x = (Uint32)(x * Blend + last_x * (1.0 - Blend));
	pos.y = 50;
	pos.w = 128;
	pos.h = 128;
	
	SDL_QueryTexture(Texture, NULL, NULL, &pos.w, &pos.h);
	SDL_RenderClear(Renderer);
	SDL_RenderCopy(Renderer, Texture, NULL, &pos);
}
