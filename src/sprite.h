#pragma once

// Libraries
#include <SDL2/SDL.h>
#include <vector2.h>
#include <physics.h>

// Sprite class
class _Sprite {

	public:

		_Sprite() : Texture(NULL) { }
		~_Sprite() { }

		void Update(float FrameTime);
		void Render(SDL_Renderer *Renderer, float Blend);
	
		float Radius;
		_Physics Physics;
		SDL_Rect Bounds;
		SDL_Texture *Texture;

};

