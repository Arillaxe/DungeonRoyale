#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include <SDL2/SDL.h>

class Texture
{
public:
  static SDL_Texture* load(const char* path);
};

#endif // !TEXTURE_HPP
