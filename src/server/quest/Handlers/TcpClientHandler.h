#ifndef QUEST_TCP_CLIENT_HANDLER_H
#define QUEST_TCP_CLIENT_HANDLER_H

#include <memory>
#include <string>
#include <sys/socket.h>
#include <unistd.h>
#include "../Interfaces/IClientHandler.h"
#include "../Interfaces/IMessageHandler.h"
#include "../Interfaces/IEncryptionService.h"
#include "../Models/ServerConfiguration.h"

namespace CloneMine {
namespace Quest {

class TcpClientHandler : public IClientHandler {
private:
    int clientSocket;
    std::shared_ptr<IMessageHandler> messageHandler;
    std::shared_ptr<IEncryptionService> encryptionService;
    ServerConfiguration config;
    bool running;
    
    std::string ReceiveMessage() {
        char buffer[4096];
        int bytesRead = recv(clientSocket, buffer, sizeof(buffer), 0);
        if (bytesRead <= 0) return "";
        
        std::string message(buffer, bytesRead);
        
        if (config.useEncryption) {
            try {
                message = encryptionService->Decrypt(message);
            } catch (...) {
                // Fallback to plaintext
            }
        }
        
        return message;
    }
    
    void SendMessage(const std::string& message) {
        std::string toSend = message;
        
        if (config.useEncryption) {
            try {
                toSend = encryptionService->Encrypt(message);
            } catch (...) {
                // Fallback to plaintext
            }
        }
        
        send(clientSocket, toSend.c_str(), toSend.length(), 0);
    }
    
public:
    TcpClientHandler(int clientSocket,
                    std::shared_ptr<IMessageHandler> messageHandler,
                    std::shared_ptr<IEncryptionService> encryptionService,
                    const ServerConfiguration& config)
        : clientSocket(clientSocket), messageHandler(messageHandler),
          encryptionService(encryptionService), config(config), running(true) {}
    
    void HandleClient() override {
        while (running) {
            std::string message = ReceiveMessage();
            if (message.empty()) break;
            
            std::string response = messageHandler->HandleMessage(message);
            SendMessage(response);
        }
        close(clientSocket);
    }
    
    void Stop() override {
        running = false;
    }
};

} // namespace Quest
} // namespace CloneMine

#endif
