#ifndef QUEST_TCP_SERVER_LISTENER_H
#define QUEST_TCP_SERVER_LISTENER_H

#include <memory>
#include <thread>
#include <vector>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include "../Interfaces/IClientHandler.h"
#include "../Models/ServerConfiguration.h"

namespace CloneMine {
namespace Quest {

class TcpServerListener {
private:
    ServerConfiguration config;
    int serverSocket;
    bool running;
    std::vector<std::thread> clientThreads;
    std::function<std::shared_ptr<IClientHandler>(int)> clientHandlerFactory;
    
public:
    TcpServerListener(const ServerConfiguration& config,
                     std::function<std::shared_ptr<IClientHandler>(int)> factory)
        : config(config), serverSocket(-1), running(false), clientHandlerFactory(factory) {}
    
    void Start() {
        serverSocket = socket(AF_INET, SOCK_STREAM, 0);
        
        sockaddr_in serverAddr{};
        serverAddr.sin_family = AF_INET;
        serverAddr.sin_addr.s_addr = INADDR_ANY;
        serverAddr.sin_port = htons(config.port);
        
        bind(serverSocket, (sockaddr*)&serverAddr, sizeof(serverAddr));
        listen(serverSocket, config.maxClients);
        
        running = true;
        
        while (running) {
            int clientSocket = accept(serverSocket, nullptr, nullptr);
            if (clientSocket < 0) continue;
            
            auto handler = clientHandlerFactory(clientSocket);
            clientThreads.emplace_back([handler]() {
                handler->HandleClient();
            });
        }
    }
    
    void Stop() {
        running = false;
        if (serverSocket >= 0) {
            close(serverSocket);
        }
        for (auto& thread : clientThreads) {
            if (thread.joinable()) thread.join();
        }
    }
};

} // namespace Quest
} // namespace CloneMine

#endif
