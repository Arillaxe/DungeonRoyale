#pragma once

#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "Ws2_32.lib")

enum PlayerAction : unsigned int {
  MOVE_UP_START = 1,
  MOVE_LEFT_START = 2,
  MOVE_RIGHT_START = 3,
  MOVE_DOWN_START = 4,
  MOVE_UP_END = 5,
  MOVE_LEFT_END = 6,
  MOVE_RIGHT_END = 7,
  MOVE_DOWN_END = 8,
};

struct PlayerPacket {
  PlayerAction action;
};

class Network
{
public:
  static int init(int port);
  static int cleanUp();
  static int recieve(PlayerPacket* packet);

private:
  static WSADATA wsaData;
  static SOCKET _socket;
  static sockaddr_in serverAddr;
};