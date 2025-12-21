#pragma once

#include "../Interfaces/IClientHandler.h"
#include "../Interfaces/IEncryptionService.h"
#include "../Models/ServerConfiguration.h"
#include <memory>
#include <functional>
#include <thread>
#include <atomic>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <cstring>
#include <iostream>

namespace CloneMine {
namespace Login {
namespace Services {

/**
 * @brief TCP server listener
 * Listens for incoming connections and spawns client handlers.
 */
class TcpServerListener {
private:
    ServerConfiguration config_;
    std::shared_ptr<IEncryptionService> encryptionService_;
    std::function<std::shared_ptr<IClientHandler>(int, std::shared_ptr<IEncryptionService>)> clientHandlerFactory_;
    std::atomic<bool> running_;
    int serverSocket_;
    std::thread listenerThread_;

    void ListenLoop() {
        while (running_) {
            struct sockaddr_in clientAddr;
            socklen_t clientLen = sizeof(clientAddr);
            
            int clientSocket = accept(serverSocket_, 
                                     (struct sockaddr*)&clientAddr, 
                                     &clientLen);
            
            if (clientSocket < 0) {
                if (running_) {
                    std::cerr << "Failed to accept client connection" << std::endl;
                }
                continue;
            }

            std::cout << "Client connected from " 
                     << inet_ntoa(clientAddr.sin_addr) << ":" 
                     << ntohs(clientAddr.sin_port) << std::endl;

            // Create client handler
            auto clientHandler = clientHandlerFactory_(clientSocket, encryptionService_);
            
            // Handle client in separate thread
            std::thread clientThread([clientHandler]() {
                try {
                    clientHandler->Handle();
                }
                catch (const std::exception& e) {
                    std::cerr << "Client handler error: " << e.what() << std::endl;
                }
            });
            
            clientThread.detach();
        }
    }

public:
    TcpServerListener(const ServerConfiguration& config,
                     std::shared_ptr<IEncryptionService> encryptionService,
                     std::function<std::shared_ptr<IClientHandler>(int, std::shared_ptr<IEncryptionService>)> clientHandlerFactory)
        : config_(config),
          encryptionService_(encryptionService),
          clientHandlerFactory_(clientHandlerFactory),
          running_(false),
          serverSocket_(-1) {
    }

    ~TcpServerListener() {
        Stop();
    }

    bool Start() {
        if (running_) {
            return false;
        }

        // Create socket
        serverSocket_ = socket(AF_INET, SOCK_STREAM, 0);
        if (serverSocket_ < 0) {
            std::cerr << "Failed to create socket" << std::endl;
            return false;
        }

        // Set socket options
        int opt = 1;
        if (setsockopt(serverSocket_, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
            std::cerr << "Failed to set socket options" << std::endl;
            close(serverSocket_);
            return false;
        }

        // Bind socket
        struct sockaddr_in serverAddr;
        memset(&serverAddr, 0, sizeof(serverAddr));
        serverAddr.sin_family = AF_INET;
        serverAddr.sin_addr.s_addr = INADDR_ANY;
        serverAddr.sin_port = htons(config_.Port);

        if (bind(serverSocket_, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
            std::cerr << "Failed to bind socket to port " << config_.Port << std::endl;
            close(serverSocket_);
            return false;
        }

        // Listen
        if (listen(serverSocket_, config_.MaxConnections) < 0) {
            std::cerr << "Failed to listen on socket" << std::endl;
            close(serverSocket_);
            return false;
        }

        std::cout << config_.ServerName << " listening on port " << config_.Port << std::endl;

        // Start listener thread
        running_ = true;
        listenerThread_ = std::thread(&TcpServerListener::ListenLoop, this);

        return true;
    }

    void Stop() {
        if (!running_) {
            return;
        }

        running_ = false;

        // Close server socket to unblock accept()
        if (serverSocket_ >= 0) {
            close(serverSocket_);
            serverSocket_ = -1;
        }

        // Wait for listener thread
        if (listenerThread_.joinable()) {
            listenerThread_.join();
        }

        std::cout << config_.ServerName << " stopped" << std::endl;
    }

    bool IsRunning() const {
        return running_;
    }
};

} // namespace Services
} // namespace Login
} // namespace CloneMine
