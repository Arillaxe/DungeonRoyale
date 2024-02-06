#include <iostream>
#include "Network.hpp"

int main() {

  Network::init(8888);

  PlayerPacket packet;

  while (1)
  {
    memset(&packet, 0, sizeof(PlayerPacket));
    Network::recieve(&packet);
    std::cout << packet.action << std::endl;
  }

  Network::cleanUp();

  return 0;
}