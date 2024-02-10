#pragma once

#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "Ws2_32.lib")

#include <thread>

#include <DungeonRoyale/GameState.hpp>

struct OutPacket {
  float x;
  float y;
};

class Network
{
public:
  static int sendToServer(PlayerPacket& packet);
  static void startReceiveThread();
  static void stopReceiveThread();
  static int recieve(OutPacket* packet);

  static bool stopThread;

private:
  static WSADATA wsaData;
  static SOCKET _socket;
  static sockaddr_in serverAddr;

  static std::thread thread;

  static int init(const char* ip, int port);
  static int cleanUp();

};