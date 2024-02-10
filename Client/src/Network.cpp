#include <iostream>
#include <DungeonRoyale/Network.hpp>
#include <cassert>
#include <DungeonRoyale/Globals.hpp>

WSADATA Network::wsaData;
SOCKET Network::_socket;
sockaddr_in Network::serverAddr;
std::thread Network::thread;
bool Network::stopThread = false;

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

  struct sockaddr_in clientAddr;
  memset(&clientAddr, 0, sizeof(clientAddr));
  clientAddr.sin_family = AF_INET;
  clientAddr.sin_addr.s_addr = htonl(INADDR_ANY); // Any local IP
  clientAddr.sin_port = htons(0); // System will assign an arbitrary port

  if (bind(_socket, (struct sockaddr*)&clientAddr, sizeof(clientAddr)) < 0) {
    printf("Bind failed with error code : %d", WSAGetLastError());
    Network::cleanUp();
    return 1;
  }

  memset((char*)&serverAddr, 0, sizeof(serverAddr));
  serverAddr.sin_family = AF_INET;
  serverAddr.sin_port = htons(port);
  inet_pton(AF_INET, ip, &serverAddr.sin_addr);

  std::cout << "Network initialized client" << std::endl;

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

  return 0;
}

int Network::recieve(OutPacket* packet) {
  struct sockaddr_in fromAddr;
  socklen_t fromAddrLen = sizeof(fromAddr);
  int receivedBytes = recvfrom(_socket, reinterpret_cast<char*>(packet), sizeof(OutPacket), 0, (struct sockaddr*)&fromAddr, &fromAddrLen);
  if (receivedBytes == SOCKET_ERROR) {
    std::cerr << "recvfrom failed with error: " << WSAGetLastError() << "\n";
    Network::cleanUp();
    return 1;
  }

  return 0;
}

int Network::cleanUp()
{
  closesocket(_socket);
  WSACleanup();

  std::cout << "Thread cleaned up" << "\n";

  return 0;
}

void Network::startReceiveThread()
{
  Network::init("127.0.0.1", 8888);

  thread = std::thread([]() {
    std::cout << "From netwroking thread" << "\n";
    while (!stopThread) {
      OutPacket packet;
      memset(&packet, 0, sizeof(OutPacket));
      Network::recieve(&packet);

      std::cout << "Received packet" << "\n";

      globals.playerPos.x = packet.x;
      globals.playerPos.y = packet.y;
    }

    std::cout << "Thread stopping" << "\n";
  });
}

void Network::stopReceiveThread()
{
  stopThread = true;
  Network::cleanUp();
  thread.join();
}

