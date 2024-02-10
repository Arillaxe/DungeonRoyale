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
  private:
    WSADATA wsaData;
    SOCKET _socket;
    sockaddr_in serverAddr;
    std::thread receivingThread;
    std::map<std::string, sockaddr_in> clients;

    std::function<void(std::string)> onConnectCallback;
    std::function<void(std::string)> onDisconnectCallback;
  public:
    Server()
    {
      int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
      if (result != 0) {
        std::cerr << "WSAStartup failed: " << result << "\n";
        cleanUp();
      }

      _socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
      if (_socket == INVALID_SOCKET) {
        std::cerr << "Socket creation failed with error: " << WSAGetLastError() << "\n";
        cleanUp();
      }
    }

    void listen(unsigned int port)
    {
      serverAddr.sin_family = AF_INET;
      serverAddr.sin_port = htons(port);
      serverAddr.sin_addr.s_addr = INADDR_ANY;

      if (bind(_socket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR)
      {
        std::cerr << "Bind failed with error: " << WSAGetLastError() << "\n";
        cleanUp();
        return;
      }

      std::cout << "Listening on port: " << port << std::endl;
    }

    void onReceive(std::function<void(std::string, InPacket)> callback)
    {
      receivingThread = std::thread([callback, this]() {
        while (true) {
          InPacket packet;
          sockaddr_in sender;

          receive(&packet, &sender);

          std::string clientKey = getClientKey(sender);

          if (clients.count(clientKey) == 0) {
            clients[clientKey] = sender;
            if (onConnectCallback) {
              onConnectCallback(clientKey);
            }
          }

          callback(clientKey, packet);
        }
        });
    }

    void send(std::string id, OutPacket* packet)
    {
      int addrSize = sizeof(sockaddr_in);
      int packetSize = sizeof(OutPacket);

      if (clients.count(id) == 0)
      {
        return;
      }

      int sentBytes = sendto(_socket, reinterpret_cast<char*>(packet), packetSize, 0, (const struct sockaddr*)&clients[id], addrSize);
      if (sentBytes == SOCKET_ERROR) {
        std::cerr << "sendto failed with error: " << WSAGetLastError() << "\n";
      }
    }

    void cleanUp()
    {
      closesocket(_socket);
      WSACleanup();
    }

    void onConnect(std::function<void(std::string id)> callback)
    {
      onConnectCallback = callback;
    }

    void onDisconnect(std::function<void(std::string id)> callback)
    {
      onDisconnectCallback = callback;
    }

  private:
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
      memset(packet, 0, packetSize);

      int addrSize = sizeof(sockaddr_in);
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