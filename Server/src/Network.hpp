#pragma once

#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "Ws2_32.lib")

#include <map>
#include <string>

inline std::string createClientKey(const sockaddr_in& cli_addr) {
  // Buffer to hold the IP address
  char ipAddr[INET_ADDRSTRLEN];

  // Convert the IP to a string and store it in ipAddr
  if (inet_ntop(AF_INET, &(cli_addr.sin_addr), ipAddr, INET_ADDRSTRLEN) == nullptr) {
    // Handle error: inet_ntop returned nullptr
    std::cerr << "Error converting IP address" << std::endl;
    return ""; // Return an empty string or handle the error as appropriate
  }

  // Convert port number from network byte order to host byte order and then to string
  unsigned short port = ntohs(cli_addr.sin_port);

  return std::string(ipAddr) + ":" + std::to_string(port);
}

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

struct Packet {
  PlayerPacket packet;
  sockaddr_in addr;
};

struct OutPacket {
  float x;
  float y;
};

class Network
{
public:
  static int init(int port);
  static int cleanUp();
  static int recieve(Packet* packet);
  static int send(sockaddr_in serverAddr, OutPacket packet);

  static std::map<std::string, sockaddr_in> clients;

private:
  static WSADATA wsaData;
  static SOCKET _socket;
  static sockaddr_in serverAddr;

  
};