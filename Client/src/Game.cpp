#include <iostream>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <DungeonRoyale/Game.hpp>
#include <DungeonRoyale/Globals.hpp>

int Game::init()
{
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
    return -1;
  }

  if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
    std::cerr << "SDL_image could not initialize! SDL_image Error: " << IMG_GetError() << std::endl;
    SDL_Quit();
    return -1;
  }

  globals.window = SDL_CreateWindow("SDL2 Demo", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, globals.SCREEN_WIDTH, globals.SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
  if (!globals.window) {
    std::cerr << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
    SDL_Quit();
    return -1;
  }

  globals.renderer = SDL_CreateRenderer(globals.window, -1, SDL_RENDERER_ACCELERATED);
  if (!globals.renderer) {
    std::cerr << "Renderer could not be created! SDL_Error: " << SDL_GetError() << std::endl;
    SDL_DestroyWindow(globals.window);
    SDL_Quit();
    return -1;
  }

  return 0;
}

void Game::cleanUp(SDL_Texture* texture)
{
  SDL_DestroyTexture(texture);
  SDL_DestroyRenderer(globals.renderer);
  SDL_DestroyWindow(globals.window);
  IMG_Quit();
  SDL_Quit();
}