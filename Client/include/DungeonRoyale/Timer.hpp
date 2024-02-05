#ifndef TIMER_HPP
#define TIMER_HPP

#include <SDL2/SDL_stdinc.h>

class Timer
{
public:
  Timer();
  Timer(int milliseconds);

  void tick();
  bool isFinished();
  void reset();
  void reset(int milliseconds);

private:
  int _milliseconds;
  int timeLeft;
  Uint64 now;
  Uint64 last;
  double deltaTime;
};

#endif // !TIMER_HPP
