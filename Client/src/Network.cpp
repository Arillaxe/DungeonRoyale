#include <iostream>
#include <DungeonRoyale/Network.hpp>

WSADATA Network::wsaData;
SOCKET Network::_socket;
sockaddr_in Network::serverAddr;

int Network::init(const char* ip, int port)
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

  memset((char*)&serverAddr, 0, sizeof(serverAddr));
  serverAddr.sin_family = AF_INET;
  serverAddr.sin_port = htons(port);
  inet_pton(AF_INET, ip, &serverAddr.sin_addr);

  std::cout << "Network initialized" << std::endl;

  return 0;
}

int Network::sendToServer(PlayerPacket& packet)
{
  int serverAddrSize = sizeof(serverAddr);
  int sentBytes = sendto(_socket, reinterpret_cast<char*>(&packet), sizeof(PlayerPacket), 0, (struct sockaddr*)&serverAddr, serverAddrSize);
  if (sentBytes == SOCKET_ERROR) {
    std::cerr << "sendto failed with error: " << WSAGetLastError() << "\n";
    Network::cleanUp();
    return 1;
  }

  std::cout << "Data sent: " << sentBytes << std::endl;

  return 0;
}

int Network::cleanUp()
{
  closesocket(_socket);
  WSACleanup();

  return 0;
}

