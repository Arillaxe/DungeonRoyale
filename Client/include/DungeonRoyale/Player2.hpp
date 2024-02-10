#ifndef PLAYER2_HPP
#define PLAYER2_HPP

#include <DungeonRoyale/Timer.hpp>
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>

struct Player2 {
public:
  Player2(float _x, float _y, float _w, float _h, SDL_Texture* _texture, SDL_Rect _srcRect);

  void tick();
  void draw();

  bool running = false;

  bool flipped = false;
  float speed = 200;
  float x;
  float y;
  int w;
  int h;


private:
  SDL_Texture* texture;
  Timer animationTimer;
  SDL_Rect srcRect;
  int animaton_frame = 2;
};

#endif // !PLAYER2_HPP
