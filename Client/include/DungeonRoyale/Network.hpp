#pragma once

#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "Ws2_32.lib")

#include <DungeonRoyale/GameState.hpp>

class Network
{
public:
  static int init(const char* ip,int port);
  static int cleanUp();
  static int sendToServer(PlayerPacket& packet);

private:
  static WSADATA wsaData;
  static SOCKET _socket;
  static sockaddr_in serverAddr;
};