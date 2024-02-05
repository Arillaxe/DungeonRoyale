#include <iostream>

#include <SDL2/SDL_image.h>

#include <DungeonRoyale/Globals.hpp>
#include <DungeonRoyale/Texture.hpp>

SDL_Texture* Texture::load(const char* path)
{
  SDL_Texture* spriteTexture = IMG_LoadTexture(globals.renderer, path);
  if (!spriteTexture) {
    std::cerr << "Failed to load texture! SDL_image Error: " << IMG_GetError() << std::endl;
  }
  return spriteTexture;
}