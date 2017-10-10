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
#include <player.h>

void _Player::Init(SDL_Texture *Texture) {
	Sprite.w = 64;
	Sprite.h = 64;
	Radius = 32;
	this->Texture = Texture;
}

void _Player::Update(float FrameTime) {
	Physics.Update(FrameTime);
	if(Physics.GetPosition().Y < 0)
		Physics.SetPosition(Vector2(Physics.GetPosition().X, 0));
}

void _Player::Jump(float Power) {
	Physics.SetVelocity(Vector2(0, Power));
}

void _Player::Render(SDL_Renderer *Renderer, float Blend) {
	const Vector2 &Position = Physics.GetPosition();
	const Vector2 &LastPosition = Physics.GetLastPosition();
	Sprite.x = (int)(Position.X * Blend + LastPosition.X * (1.0f - Blend) + 0.5f) - 32;
	Sprite.y = (int)(Position.Y * Blend + LastPosition.Y * (1.0f - Blend) + 0.5f) - 32;

	SDL_RenderCopy(Renderer, Texture, nullptr, &Sprite);
}
