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
