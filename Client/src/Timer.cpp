#include <SDL2/SDL_timer.h>

#include <DungeonRoyale/Timer.hpp>

Timer::Timer(int milliseconds)
{
  _milliseconds = timeLeft = milliseconds;
  now = SDL_GetPerformanceCounter();
  last = 0;
  deltaTime = 0;
}

void Timer::tick()
{
  last = now;
  now = SDL_GetPerformanceCounter();
  deltaTime = (double)((now - last) * 1000 / (double)SDL_GetPerformanceFrequency()) * 0.001;

  timeLeft -= deltaTime * 1000;
}

bool Timer::isFinished()
{
  return timeLeft <= 0;
}

void Timer::reset()
{
  timeLeft = _milliseconds;
}

void Timer::reset(int milliseconds)
{
  _milliseconds = timeLeft = milliseconds;
}