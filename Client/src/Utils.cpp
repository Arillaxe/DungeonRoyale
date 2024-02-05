#include <SDL2/SDL_timer.h>

#include <DungeonRoyale/Utils.hpp>

Uint64 Utils::now = SDL_GetPerformanceCounter();
Uint64 Utils::last = 0;
double Utils::deltaTime = 0;
Uint32 Utils::lastTime = SDL_GetTicks();
int Utils::frameCount = 0;

int Utils::getFPS()
{
  return frameCount;
}

void Utils::updateFPS()
{
  frameCount++;
  if (SDL_GetTicks() - lastTime > 1000) {
    lastTime = SDL_GetTicks();
    frameCount = 0;
  }
}

double Utils::getDeltaTime()
{
  return deltaTime;
}

void Utils::updateDeltaTime()
{
  last = now;
  now = SDL_GetPerformanceCounter();
  deltaTime = (double)((now - last) * 1000 / (double)SDL_GetPerformanceFrequency()) * 0.001;
}