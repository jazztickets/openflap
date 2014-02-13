#include <SDL2/SDL.h>
#include <iostream>

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
	
	SDL_Renderer *Renderer = NULL;
	Renderer = SDL_CreateRenderer(Window, -1, SDL_RENDERER_PRESENTVSYNC);
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
	
	SDL_Texture *Texture = NULL;
	Texture = SDL_CreateTextureFromSurface(Renderer, Image);
	SDL_FreeSurface(Image);
	
	bool Quit = false;
	SDL_Event Event;
    while(!Quit) {
		while(SDL_PollEvent(&Event)) {
			if(Event.type == SDL_QUIT)
				Quit = true;
			if(Event.type == SDL_KEYDOWN)
				Quit = true;
		}

		SDL_Rect pos;
		pos.x = 525;
		pos.y = 50;
		pos.w = 128;
		pos.h = 128;
		SDL_QueryTexture(Texture, NULL, NULL, &pos.w, &pos.h);
		SDL_RenderClear(Renderer);
		SDL_RenderCopy(Renderer, Texture, NULL, &pos);
		SDL_RenderPresent(Renderer);
	}
		
	SDL_DestroyTexture(Texture);
	SDL_DestroyRenderer(Renderer);
	SDL_DestroyWindow(Window);
	SDL_Quit();
	
	return 0;
}
