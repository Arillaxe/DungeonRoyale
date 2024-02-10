#pragma once

#include <map>
#include "Player.hpp"

struct Globals {
  std::map<std::string, ServerPlayer> players;
};

extern Globals globals;