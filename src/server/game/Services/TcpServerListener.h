#pragma once

#include "../Models/ServerConfiguration.h"
#include "../Interfaces/IClientHandler.h"
#include <memory>
#include <thread>
#include <vector>
#include <atomic>

#ifdef _WIN32
#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")
#else
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#endif

namespace CloneMine {
namespace Game {
namespace Services {

class TcpServerListener {
private:
    std::shared_ptr<Models::ServerConfiguration> config;
    std::shared_ptr<Interfaces::IClientHandler> clientHandlerTemplate;
    int serverSocket;
    std::atomic<bool> running;
    std::vector<std::thread> clientThreads;

    void InitializeSocket() {
#ifdef _WIN32
        WSADATA wsaData;
        WSAStartup(MAKEWORD(2, 2), &wsaData);
#endif

        serverSocket = socket(AF_INET, SOCK_STREAM, 0);
        
        int opt = 1;
#ifdef _WIN32
        setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, (char*)&opt, sizeof(opt));
#else
        setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
#endif

        sockaddr_in serverAddr{};
        serverAddr.sin_family = AF_INET;
        serverAddr.sin_addr.s_addr = INADDR_ANY;
        serverAddr.sin_port = htons(config->Port);

        bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
        listen(serverSocket, 10);
    }

public:
    TcpServerListener(std::shared_ptr<Models::ServerConfiguration> configuration,
                     std::shared_ptr<Interfaces::IClientHandler> clientHandler)
        : config(configuration),
          clientHandlerTemplate(clientHandler),
          serverSocket(-1),
          running(false) {
    }

    ~TcpServerListener() {
        Stop();
#ifdef _WIN32
        if (serverSocket != -1) closesocket(serverSocket);
        WSACleanup();
#else
        if (serverSocket != -1) close(serverSocket);
#endif
    }

    void Start() {
        if (running) return;

        InitializeSocket();
        running = true;

        while (running) {
            sockaddr_in clientAddr{};
            socklen_t clientLen = sizeof(clientAddr);
            
#ifdef _WIN32
            int clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddr, &clientLen);
            if (clientSocket == INVALID_SOCKET) {
#else
            int clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddr, &clientLen);
            if (clientSocket < 0) {
#endif
                if (running) continue;
                else break;
            }

            // Handle client in new thread
            clientThreads.emplace_back([this, clientSocket]() {
                clientHandlerTemplate->HandleClient(clientSocket);
            });
        }

        // Wait for all client threads to finish
        for (auto& thread : clientThreads) {
            if (thread.joinable()) {
                thread.join();
            }
        }
    }

    void Stop() {
        running = false;
#ifdef _WIN32
        if (serverSocket != -1) closesocket(serverSocket);
#else
        if (serverSocket != -1) close(serverSocket);
#endif
    }
};

} // namespace Services
} // namespace Game
} // namespace CloneMine
