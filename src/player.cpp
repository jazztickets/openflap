#include <player.h>

void _Player::Init(SDL_Texture *Texture) {
	Sprite.w = 64;
	Sprite.h = 64;
	Radius = 30;
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
	Sprite.x = (Uint32)(Position.X * Blend + LastPosition.X * (1.0f - Blend) + 0.5f) - Radius - 3;
	Sprite.y = (Uint32)(Position.Y * Blend + LastPosition.Y * (1.0f - Blend) + 0.5f) - Radius - 1;

	SDL_RenderCopy(Renderer, Texture, NULL, &Sprite);
}
