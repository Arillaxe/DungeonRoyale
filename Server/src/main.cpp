#include <iostream>
#include <map>
#include <string>
#include <chrono>
#include <thread>

#include <Network/Server.hpp>

struct PlayerInputStates {
  bool up = false;
  bool left = false;
  bool down = false;
  bool right = false;
};

std::map<std::string, PlayerInputStates> playerStates;

std::thread thread;

void handleInputState(std::string clientKey, PlayerAction action) {
  auto& playerState = playerStates[clientKey];
  
  switch (action) {
    case PlayerAction::MOVE_UP_START:
    case PlayerAction::MOVE_UP_END:
      playerState.up = action == PlayerAction::MOVE_UP_START;
      break;
    case PlayerAction::MOVE_LEFT_START:
    case PlayerAction::MOVE_LEFT_END:
      playerState.left = action == PlayerAction::MOVE_LEFT_START;
      break;
    case PlayerAction::MOVE_DOWN_START:
    case PlayerAction::MOVE_DOWN_END:
      playerState.down = action == PlayerAction::MOVE_DOWN_START;
      break;
    case PlayerAction::MOVE_RIGHT_START:
    case PlayerAction::MOVE_RIGHT_END:
      playerState.right = action == PlayerAction::MOVE_RIGHT_START;
      break;
  }
}

struct PlayerPosition {
  float x = 0;
  float y = 0;
};

std::map<std::string, PlayerPosition> playerPositions;

float playerSpeed = 200;

void handlePositions(double deltaTime) {
  for (auto& pair : playerPositions) {
    if (playerStates.count(pair.first) == 0) {
      continue;
    }

    PlayerInputStates& states = playerStates[pair.first];
    float x = (states.right - states.left) * playerSpeed * deltaTime;
    float y = (states.down - states.up) * playerSpeed * deltaTime;

    pair.second.x += x;
    pair.second.y += y;
  }
}

int main() {

  Network::init(8888);

  auto lastFrameTime = std::chrono::high_resolution_clock::now();
  double deltaTime = 0.0;

  float tickrateTimer = 0.015625f;
  float timeLeft = tickrateTimer;

  thread = std::thread([]() {
    while (true) {
      Packet packet;
      memset(&packet, 0, sizeof(Packet));
      Network::recieve(&packet);

      std::cout << "received packet" << "\n";

      std::string clientKey = createClientKey(packet.addr);

      if (strcmp(clientKey.c_str(), "0.0.0.0:0") != 0) {
        if (playerStates.count(clientKey) == 0) {
          playerStates[clientKey] = PlayerInputStates{};
        }

        if (playerPositions.count(clientKey) == 0) {
          playerPositions[clientKey] = PlayerPosition{};
        }

        handleInputState(clientKey, packet.packet.action);
      };
    }
  });

  while (true)
  {
    auto currentFrameTime = std::chrono::high_resolution_clock::now();
    deltaTime = std::chrono::duration<double, std::chrono::seconds::period>(currentFrameTime - lastFrameTime).count();
    lastFrameTime = currentFrameTime;

    timeLeft -= deltaTime;

    handlePositions(deltaTime);

    if (timeLeft <= 0) {
      timeLeft = tickrateTimer;

      for (const auto& pair : Network::clients) {
        if (playerPositions.count(pair.first) == 0) {
          continue;
        }

        PlayerPosition& pos = playerPositions[pair.first];

        OutPacket packet = OutPacket{ pos.x, pos.y };

        Network::send(pair.second, packet);
      }
    }
  }

  Network::cleanUp();

  thread.join();

  return 0;
}