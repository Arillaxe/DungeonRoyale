#ifndef GAME_HPP
#define GAME_HPP

class Game
{
public:
  static int init();
  static void cleanUp(SDL_Texture* texture);
};

#endif // !GAME_HPP
