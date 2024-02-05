#ifndef TILE_HPP
#define TILE_HPP

struct Tile {
  int x;
  int y;
  int type;
  bool colliding = false;
};

#endif // !TILE_HPP
