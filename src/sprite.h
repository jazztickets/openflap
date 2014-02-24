/******************************************************************************
* openflap
* Copyright (C) 2014  Alan Witkowski
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
*******************************************************************************/
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

