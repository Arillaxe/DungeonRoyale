#include <iostream>
#include <chrono>

#include <Server/Globals.hpp>
#include <Server/Server.hpp>

const float playerSpeed = 200;

void handlePositions(double deltaTime) {
  for (auto& pair : globals.players) {
    ServerPlayer& player = pair.second;
    float x = (player.right - player.left) * playerSpeed * deltaTime;
    float y = (player.down - player.up) * playerSpeed * deltaTime;

    pair.second.x += x;
    pair.second.y += y;
  }
}

struct GameStatePacket {
  int players_size = 10;
  ClientPlayer players[10];
};

struct PlayerStatePacket {
  bool up = false;
  bool left = false;
  bool down = false;
  bool right = false;
};

void onConnect(std::string id)
{
  std::cout << "Connected: " << id << "\n";

  globals.players[id] = ServerPlayer{ id };
}

void onDisconnect(std::string id)
{
  std::cout << "Disonnected: " << id << "\n";

  globals.players.erase(id);
}

void onReceive(std::string id, PlayerStatePacket packet) {
  globals.players[id].up = packet.up;
  globals.players[id].left = packet.left;
  globals.players[id].down = packet.down;
  globals.players[id].right = packet.right;
}

int main() {

  Network::Server<GameStatePacket, PlayerStatePacket> server;

  server.listen(8888);

  server.onConnect(onConnect);
  server.onDisconnect(onDisconnect);
  server.onReceive(onReceive);

  auto lastFrameTime = std::chrono::high_resolution_clock::now();
  double deltaTime = 0.0;

  float tickrateTimer = 0.015625f;
  float timeLeft = tickrateTimer;

  while (true)
  {
    auto currentFrameTime = std::chrono::high_resolution_clock::now();
    deltaTime = std::chrono::duration<double, std::chrono::seconds::period>(currentFrameTime - lastFrameTime).count();
    lastFrameTime = currentFrameTime;

    timeLeft -= deltaTime;

    handlePositions(deltaTime);

    if (timeLeft <= 0) {
      timeLeft = tickrateTimer;


      GameStatePacket packet;

      int i = 0;
      for (const auto& pair : globals.players) {
        if (i < 10) {
          packet.players[i++] = ClientPlayer{ pair.first, pair.second.x, pair.second.y };
        }
      }

      for (const auto& pair : globals.players) {
        server.send(pair.first, &packet);
      }
    }
  }

  server.cleanUp();

  return 0;
}