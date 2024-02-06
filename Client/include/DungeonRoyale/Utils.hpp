#ifndef UTILS_HPP
#define UTILS_HPP

#include <SDL2/SDL_stdinc.h>

class Utils
{
public:
  static int getFPS();
  static void updateFPS();
  static double getDeltaTime();
  static void updateDeltaTime();
private:
  static Uint64 now;
  static Uint64 last;
  static double deltaTime;

  static Uint32 lastTime;
  static int frameCount;
  static int fps;
};

#endif // !UTILS_HPP
