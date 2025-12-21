#pragma once

#include "../Interfaces/IChatService.h"
#include "../Models/ServerConfiguration.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <thread>
#include <vector>
#include <memory>
#include <functional>

namespace CloneMine {
namespace Chat {

/**
 * @brief TCP server listener for chat server
 * 
 * Follows Single Responsibility Principle - only handles TCP connections
 * Follows Open/Closed Principle - extensible via client handler callback
 */
class TcpServerListener {
private:
    ServerConfiguration config;
    std::shared_ptr<IChatService> chatService;
    int serverSocket;
    bool running;
    std::vector<std::thread> clientThreads;
    std::function<void(int)> clientHandler;

public:
    TcpServerListener(const ServerConfiguration& config,
                      std::shared_ptr<IChatService> chatService)
        : config(config), chatService(chatService), serverSocket(-1), running(false) {}

    ~TcpServerListener() {
        Stop();
    }

    void SetClientHandler(std::function<void(int)> handler) {
        clientHandler = handler;
    }

    void Start() {
        serverSocket = socket(AF_INET, SOCK_STREAM, 0);
        if (serverSocket < 0) {
            throw std::runtime_error("Failed to create socket");
        }

        int opt = 1;
        setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

        sockaddr_in serverAddr{};
        serverAddr.sin_family = AF_INET;
        serverAddr.sin_addr.s_addr = INADDR_ANY;
        serverAddr.sin_port = htons(config.port);

        if (bind(serverSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
            close(serverSocket);
            throw std::runtime_error("Failed to bind socket");
        }

        if (listen(serverSocket, 10) < 0) {
            close(serverSocket);
            throw std::runtime_error("Failed to listen on socket");
        }

        running = true;
        chatService->Start();

        // Accept connections loop
        while (running) {
            sockaddr_in clientAddr{};
            socklen_t clientLen = sizeof(clientAddr);
            int clientSocket = accept(serverSocket, (sockaddr*)&clientAddr, &clientLen);

            if (clientSocket < 0) {
                if (running) {
                    continue;
                }
                break;
            }

            // Handle client in new thread
            if (clientHandler) {
                clientThreads.emplace_back(clientHandler, clientSocket);
            }
        }
    }

    void Stop() {
        running = false;
        chatService->Stop();

        if (serverSocket >= 0) {
            close(serverSocket);
            serverSocket = -1;
        }

        // Wait for all client threads
        for (auto& thread : clientThreads) {
            if (thread.joinable()) {
                thread.join();
            }
        }
        clientThreads.clear();
    }
};

} // namespace Chat
} // namespace CloneMine
