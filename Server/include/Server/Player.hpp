#pragma once

#include <string>

struct ClientPlayer {
  std::string id;
  float x;
  float y;
};

struct ServerPlayer {
  std::string id;
  float x = 0;
  float y = 0;
  bool up = false;
  bool left = false;
  bool down = false;
  bool right = false;
};