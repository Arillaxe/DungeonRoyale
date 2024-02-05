#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <DungeonRoyale/Timer.hpp>
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>

struct Player {
public:
  Player(SDL_Rect _rect, SDL_Texture* _texture, SDL_Rect _srcRect);

  void tick();
  void draw();

  bool running = false;

  bool flipped = false;
  float speed = 200;
  SDL_Rect rect;

private:
  SDL_Texture* texture;
  Timer animationTimer;
  SDL_Rect srcRect;
  int animaton_frame = 2;
};

#endif // !PLAYER_HPP
