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
  class Server {
  public:
    Server(unsigned int port)
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
      serverAddr.sin_addr.s_addr = INADDR_ANY;
    }

    void init()
    {
      if (bind(_socket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR)
      {
        std::cerr << "Bind failed with error: " << WSAGetLastError() << "\n";
        cleanUp();
        return;
      }

      std::cout << "Network initialized" << std::endl;
    }

    void sendAll(OutPacket* packet)
    {
      int addrSize = sizeof(sockaddr_in);
      int packetSize = sizeof(OutPacket);

      for (const auto& pair : clients) {
        int sentBytes = sendto(_socket, reinterpret_cast<char*>(packet), packetSize, 0, (const struct sockaddr*)&pair.second, &addrSize);
        if (sentBytes == SOCKET_ERROR) {
          std::cerr << "sendto failed with error: " << WSAGetLastError() << "\n";
          return 1;
        }
      }
    }

    void startReceiving(std::function<void(InPacket)> callback)
    {
      receivingThread = std::thread([]() {
        while (true) {
          InPacket packet;
          sockaddr_in sender;

          receive(&packet, &sender);

          std::string clientKey = getClientKey(sender);

          if (clients.count(clientKey) == 0) {
            clients[clientKey] = sender;
          }

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
    std::map<std::string, sockaddr_in> clients;

    std::string getClientKey(sockaddr_in& addr)
    {
      char ipAddr[INET_ADDRSTRLEN];

      if (inet_ntop(AF_INET, &(addr.sin_addr), ipAddr, INET_ADDRSTRLEN) == nullptr) {
        std::cerr << "Error converting IP address" << std::endl;
        return "";
      }

      unsigned short port = ntohs(addr.sin_port);

      return std::string(ipAddr) + ":" + std::to_string(port);
    }

    void receive(InPacket* packet, sockaddr_in* sender)
    {
      int packetSize = sizeof(InPacket);
      memset(packet, 0, packetSize)
      int addrSize = sizeof(sockaddr_in)
      memset(sender, 0, addrSize);
      int recievedBytes = recvfrom(_socket, reinterpret_cast<char*>(packet), packetSize, 0, (struct sockaddr*)sender, &addrSize);
      if (recievedBytes == SOCKET_ERROR) {
        std::cerr << "recvfrom failed with error: " << WSAGetLastError() << "\n";
        cleanUp();
        return;
      }
    }
  };
}