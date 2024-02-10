#include <iostream>
#include "Network.hpp"
#include <cassert>

WSADATA Network::wsaData;
SOCKET Network::_socket;
sockaddr_in Network::serverAddr;

std::map<std::string, sockaddr_in> Network::clients;

int Network::init(int port)
{
  if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
    std::cerr << "WSAStartup failed.\n";
    return 1;
  }

  _socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
  if (_socket == INVALID_SOCKET) {
    std::cerr << "Socket creation failed with error: " << WSAGetLastError() << "\n";
    cleanUp();
    return 1;
  }

  serverAddr.sin_family = AF_INET;
  serverAddr.sin_port = htons(port);
  serverAddr.sin_addr.s_addr = INADDR_ANY;

  if (bind(_socket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR)
  {
    printf("Bind failed with error code : %d", WSAGetLastError());
    cleanUp();
    return 1;
  }

  std::cout << "Network initialized server" << std::endl;

  return 0;
}

int Network::recieve(Packet* packet)
{
  PlayerPacket plPacket;
  memset(&plPacket, 0, sizeof(PlayerPacket));

  int serverAddrSize = sizeof(serverAddr);
  memset(&serverAddr, 0, serverAddrSize);
  int recievedBytes = recvfrom(_socket, reinterpret_cast<char*>(&plPacket), sizeof(PlayerPacket), 0, (struct sockaddr*)&serverAddr, &serverAddrSize);
  if (recievedBytes == SOCKET_ERROR) {
    std::cerr << "recvfrom failed with error: " << WSAGetLastError() << "\n";
    cleanUp();
    return 1;
  }

  std::string clientKey = createClientKey(serverAddr);

  if (clients.count(clientKey) == 0) {
    clients[clientKey] = serverAddr;
  }

  packet->packet = plPacket;
  packet->addr = serverAddr;

  return 0;
}

int Network::send(sockaddr_in addr, OutPacket packet)
{
  socklen_t len = sizeof(addr);
  int sentBytes = sendto(_socket, reinterpret_cast<char*>(&packet), sizeof(OutPacket), 0, (const struct sockaddr*)&addr, len);
  if (sentBytes == SOCKET_ERROR) {
    std::cerr << "sendto failed with error: " << WSAGetLastError() << "\n";
    return 1;
  }

  return 0;
}

int Network::cleanUp()
{
  closesocket(_socket);
  WSACleanup();

  return 0;
}