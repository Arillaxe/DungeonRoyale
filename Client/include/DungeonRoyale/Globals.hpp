#ifndef GLOBALS_HPP
#define GLOBALS_HPP

#include <SDL2/SDL.h>

#include <DungeonRoyale/Camera.hpp>
#include <DungeonRoyale/GameState.hpp>

struct Globals {
  int SCREEN_WIDTH;
  int SCREEN_HEIGHT;
  int GRID_SIZE;
  bool DRAW_GRID;

  Camera camera;

  PlayerPos playerPos;

  SDL_Window* window = nullptr;
  SDL_Renderer* renderer = nullptr;
};

extern Globals globals;

#endif // !GLOBALS_HPP
