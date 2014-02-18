#pragma once

// Libraries
#include <SDL2/SDL.h>
#include <vector2.h>
#include <physics.h>

// Player data
class _Player {

	public:

		_Player(const _Physics &Physics) : Texture(NULL), Physics(Physics) {
		}

		~_Player() {
		}

		void Init(SDL_Texture *Texture);
		void Update(float FrameTime);
		void Jump(float Power);
		void Render(SDL_Renderer *Renderer, float Blend);
	
		float Radius;
		_Physics Physics;
		SDL_Rect Sprite;
		SDL_Texture *Texture;

};

