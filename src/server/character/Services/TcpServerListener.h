#pragma once
#include "../Interfaces/IClientHandler.h"
#include "../Models/ServerConfiguration.h"
#include <memory>
#include <thread>
#include <vector>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <iostream>

class TcpServerListener {
private:
    ServerConfiguration config;
    std::shared_ptr<IClientHandler> clientHandler;
    int serverSocket;
    bool running;
    std::vector<std::thread> clientThreads;
    
public:
    TcpServerListener(const ServerConfiguration& cfg, std::shared_ptr<IClientHandler> handler)
        : config(cfg), clientHandler(handler), serverSocket(-1), running(false) {}
    
    ~TcpServerListener() {
        Stop();
    }
    
    bool Start() {
        serverSocket = socket(AF_INET, SOCK_STREAM, 0);
        if (serverSocket < 0) {
            std::cerr << "Failed to create socket" << std::endl;
            return false;
        }
        
        int opt = 1;
        setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
        
        sockaddr_in serverAddr{};
        serverAddr.sin_family = AF_INET;
        serverAddr.sin_addr.s_addr = INADDR_ANY;
        serverAddr.sin_port = htons(config.port);
        
        if (bind(serverSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
            std::cerr << "Failed to bind socket" << std::endl;
            close(serverSocket);
            return false;
        }
        
        if (listen(serverSocket, config.maxClients) < 0) {
            std::cerr << "Failed to listen on socket" << std::endl;
            close(serverSocket);
            return false;
        }
        
        running = true;
        std::cout << "CharacterServer listening on port " << config.port << std::endl;
        
        while (running) {
            sockaddr_in clientAddr{};
            socklen_t clientLen = sizeof(clientAddr);
            int clientSocket = accept(serverSocket, (sockaddr*)&clientAddr, &clientLen);
            
            if (clientSocket < 0) {
                if (running) {
                    std::cerr << "Failed to accept client" << std::endl;
                }
                continue;
            }
            
            std::thread clientThread([this, clientSocket]() {
                clientHandler->HandleClient(clientSocket);
            });
            clientThread.detach();
        }
        
        return true;
    }
    
    void Stop() {
        running = false;
        if (serverSocket >= 0) {
            close(serverSocket);
            serverSocket = -1;
        }
    }
};
