#include <SDL2/SDL_timer.h>

#include <DungeonRoyale/Utils.hpp>
#include <iostream>

Uint64 Utils::now = SDL_GetPerformanceCounter();
Uint64 Utils::last = 0;
double Utils::deltaTime = 0;
Uint32 Utils::lastTime = SDL_GetTicks();
int Utils::frameCount = 0;
int Utils::fps = 0;

int Utils::getFPS()
{
  return fps;
}

void Utils::updateFPS()
{
  Uint32 currentTicks = SDL_GetTicks();
  frameCount++;

  if (currentTicks - lastTime > 50) {
    fps = frameCount * (1000 / 50);
    lastTime = currentTicks;
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