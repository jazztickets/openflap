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
#include <sprite.h>

void _Sprite::Update(float FrameTime) {
	Physics.Update(FrameTime);
}

void _Sprite::Render(SDL_Renderer *Renderer, float Blend) {
	const Vector2 &Position = Physics.GetPosition();
	const Vector2 &LastPosition = Physics.GetLastPosition();
	Bounds.x = (Uint32)(Position.X * Blend + LastPosition.X * (1.0f - Blend) + 0.5f);
	Bounds.y = (Uint32)(Position.Y * Blend + LastPosition.Y * (1.0f - Blend) + 0.5f);

	SDL_RenderCopy(Renderer, Texture, NULL, &Bounds);
}
