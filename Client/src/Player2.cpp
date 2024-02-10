#include <DungeonRoyale/Player2.hpp>

#include <DungeonRoyale/Globals.hpp>

Player2::Player2(float _x, float _y, float _w, float _h, SDL_Texture* _texture, SDL_Rect _srcRect):
  x(_x), y(_y), w(_w), h(_h), texture(_texture), srcRect(_srcRect)
{
  animationTimer = Timer(350);
}

void Player2::tick()
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

void Player2::draw()
{
  SDL_Rect srcRect = { 12 * animaton_frame, 320, 12, 15 };
  SDL_Rect destRect = {
    x - globals.camera.x + globals.SCREEN_WIDTH / 2 - w / 2,
    y - globals.camera.y + globals.SCREEN_HEIGHT / 2 - h / 2,
    w,
    h
  };

  SDL_RendererFlip flipFlags = flipped ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE;

  SDL_RenderCopyEx(globals.renderer, texture, &srcRect, &destRect, 0, NULL, flipFlags);
}