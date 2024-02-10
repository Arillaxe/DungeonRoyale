#pragma once
#pragma comment(lib, "Ws2_32.lib")

#include <winsock2.h>
#include <ws2tcpip.h>

#include <map>
#include <string>
#include <functional>
#include <thread>

namespace Network {
  template<typename OutPacket, typename InPacket>
  class Client {
  public:
    Client(const char* ip, unsigned int port)
    {
      int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
      if (result != 0) {
        std::cerr << "WSAStartup failed: " << result << "\n";
        cleanUp();
        return;
      }

      _socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
      if (_socket == INVALID_SOCKET) {
        std::cerr << "Socket creation failed with error: " << WSAGetLastError() << "\n";
        cleanUp();
        return 1;
      }

      serverAddr.sin_family = AF_INET;
      serverAddr.sin_port = htons(port);
      inet_pton(AF_INET, ip, &serverAddr.sin_addr);
    }

    void init()
    {
      struct sockaddr_in clientAddr;
      memset(&clientAddr, 0, sizeof(clientAddr));
      clientAddr.sin_family = AF_INET;
      clientAddr.sin_addr.s_addr = htonl(INADDR_ANY);
      clientAddr.sin_port = htons(0);

      if (bind(_socket, (struct sockaddr*)&clientAddr, sizeof(clientAddr)) == SOCKET_ERROR)
      {
        std::cerr << "Bind failed with error: " << WSAGetLastError() << "\n";
        cleanUp();
        return;
      }
    }

    void send(OutPacket* packet)
    {
      int addrSize = sizeof(sockaddr_in);
      int packetSize = sizeof(OutPacket);

      int sentBytes = sendto(_socket, reinterpret_cast<char*>(packet), packetSize, 0, (const struct sockaddr*)&serverAddr, &addrSize);
      if (sentBytes == SOCKET_ERROR) {
        std::cerr << "sendto failed with error: " << WSAGetLastError() << "\n";
        return 1;
      }
    }

    void startReceiving(std::function<void(InPacket)> callback)
    {
      receivingThread = std::thread([]() {
        while (true) {
          InPacket packet;

          receive(&packet);

          callback(packet);
        }
      });
    }

    void cleanUp()
    {
      closesocket(_socket);
      WSACleanup();
    }

  private:
    WSADATA wsaData;
    SOCKET _socket;
    sockaddr_in serverAddr;
    std::thread receivingThread;

    void receive(InPacket* packet)
    {
      sockaddr_in fromAddr;
      socklen_t fromAddrLen = sizeof(fromAddr);

      int packetSize = sizeof(InPacket);
      memset(packet, 0, packetSize)

      int addrSize = sizeof(sockaddr_in)
      memset(sender, 0, addrSize);

      int recievedBytes = recvfrom(_socket, reinterpret_cast<char*>(packet), packetSize, 0, (struct sockaddr*)fromAddr, &fromAddrLen);
      if (recievedBytes == SOCKET_ERROR) {
        std::cerr << "recvfrom failed with error: " << WSAGetLastError() << "\n";
        cleanUp();
        return;
      }
    }
  };
}