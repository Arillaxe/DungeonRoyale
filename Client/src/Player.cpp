#include <DungeonRoyale/Player.hpp>

#include <DungeonRoyale/Globals.hpp>

Player::Player(SDL_Rect _rect, SDL_Texture* _texture, SDL_Rect _srcRect)
{
	rect = _rect;
	texture = _texture;
	srcRect = _srcRect;

	animationTimer = Timer(100);
}

void Player::tick()
{
	animationTimer.tick();

  if (running && animationTimer.isFinished()) {
    animationTimer.reset();

    if (animaton_frame == 0) {
      animaton_frame = 1;
    }

    animaton_frame++;
    if (animaton_frame == 8) {
      animaton_frame = 2;
    }
  }

  if (!running) {
    animaton_frame = 0;
  }
}

void Player::draw()
{
  SDL_Rect srcRect = { 12 * animaton_frame, 320, 12, 15 };
  SDL_Rect destRect = {
    (int) rect.x - globals.camera.x + globals.SCREEN_WIDTH / 2 - rect.w / 2,
    (int) rect.y - globals.camera.y + globals.SCREEN_HEIGHT / 2 - rect.h / 2,
    rect.w,
    rect.h
  };

  SDL_RendererFlip flipFlags = flipped ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE;

  SDL_RenderCopyEx(globals.renderer, texture, &srcRect, &destRect, 0, NULL, flipFlags);
}