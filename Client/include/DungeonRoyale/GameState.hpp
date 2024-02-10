#pragma once

#include <DungeonRoyale/Player.hpp>

enum PlayerAction : unsigned int {
  IDLE = -1,
  MOVE_UP_START = 1,
  MOVE_LEFT_START = 2,
  MOVE_RIGHT_START = 3,
  MOVE_DOWN_START = 4,
  MOVE_UP_END = 5,
  MOVE_LEFT_END = 6,
  MOVE_RIGHT_END = 7,
  MOVE_DOWN_END = 8,
};

struct PlayerPos {
  float x;
  float y;
};

struct GameState {
  PlayerPos player_positions[10];
};

struct PlayerPacket {
  PlayerAction action;
};

