#include <iostream>
#include "Network.hpp"

WSADATA Network::wsaData;
SOCKET Network::_socket;
sockaddr_in Network::serverAddr;

int Network::init(int port)
{
  if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
    std::cerr << "WSAStartup failed.\n";
    return 1;
  }

  _socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
  if (_socket == INVALID_SOCKET) {
    std::cerr << "Socket creation failed with error: " << WSAGetLastError() << "\n";
    Network::cleanUp();
    return 1;
  }


  serverAddr.sin_family = AF_INET;
  serverAddr.sin_port = htons(port);
  serverAddr.sin_addr.s_addr = INADDR_ANY;

  if (bind(_socket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR)
  {
    printf("Bind failed with error code : %d", WSAGetLastError());
    Network::cleanUp();
    return 1;
  }

  std::cout << "Network initialized" << std::endl;

  return 0;
}

int Network::recieve(PlayerPacket* packet)
{
  int serverAddrSize = sizeof(serverAddr);
  int recievedBytes = recvfrom(_socket, reinterpret_cast<char*>(packet), sizeof(PlayerPacket), 0, (struct sockaddr*)&serverAddr, &serverAddrSize);
  if (recievedBytes == SOCKET_ERROR) {
    std::cerr << "recvfrom  failed with error: " << WSAGetLastError() << "\n";
    Network::cleanUp();
    return 1;
  }

  std::cout << "Data recieved: " << recievedBytes << std::endl;

  return 0;
}

int Network::cleanUp()
{
  closesocket(_socket);
  WSACleanup();

  return 0;
}