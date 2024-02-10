#ifndef TIMER_HPP
#define TIMER_HPP

#include <SDL2/SDL_stdinc.h>

class Timer
{
public:
  Timer() {};

  Timer(int milliseconds);

  void tick();
  bool isFinished();
  void reset();
  void reset(int milliseconds);

private:
  int _milliseconds = 0;
  int timeLeft = 0;
  Uint64 now = 0;
  Uint64 last = 0;
  double deltaTime = 0;
};

#endif // !TIMER_HPP
