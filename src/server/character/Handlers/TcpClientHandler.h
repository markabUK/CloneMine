#pragma once
#include "../Interfaces/IClientHandler.h"
#include "../Interfaces/IMessageHandler.h"
#include "../Interfaces/IEncryptionService.h"
#include <memory>
#include <sys/socket.h>
#include <unistd.h>
#include <vector>
#include <cstring>
#include <iostream>

class TcpClientHandler : public IClientHandler {
private:
    std::shared_ptr<IMessageHandler> messageHandler;
    std::shared_ptr<IEncryptionService> encryptionService;
    bool enableEncryption;
    
public:
    TcpClientHandler(std::shared_ptr<IMessageHandler> msgHandler,
                    std::shared_ptr<IEncryptionService> encService,
                    bool encryption)
        : messageHandler(msgHandler), encryptionService(encService), 
          enableEncryption(encryption) {}
    
    void HandleClient(int clientSocket) override {
        char buffer[4096];
        
        while (true) {
            // Read message length (4 bytes)
            uint32_t messageLength;
            ssize_t bytesRead = recv(clientSocket, &messageLength, sizeof(messageLength), 0);
            if (bytesRead <= 0) {
                break;
            }
            
            messageLength = ntohl(messageLength);
            if (messageLength > sizeof(buffer)) {
                std::cerr << "Message too large" << std::endl;
                break;
            }
            
            // Read message data
            bytesRead = recv(clientSocket, buffer, messageLength, 0);
            if (bytesRead <= 0) {
                break;
            }
            
            std::string message;
            
            if (enableEncryption) {
                try {
                    std::vector<unsigned char> encryptedData(buffer, buffer + bytesRead);
                    message = encryptionService->Decrypt(encryptedData);
                } catch (const std::exception& e) {
                    // Fallback to plaintext
                    message = std::string(buffer, bytesRead);
                }
            } else {
                message = std::string(buffer, bytesRead);
            }
            
            // Handle message
            std::string response = messageHandler->HandleMessage(message);
            
            // Send response
            std::vector<unsigned char> responseData;
            if (enableEncryption) {
                try {
                    responseData = encryptionService->Encrypt(response);
                } catch (const std::exception& e) {
                    responseData.assign(response.begin(), response.end());
                }
            } else {
                responseData.assign(response.begin(), response.end());
            }
            
            uint32_t responseLength = htonl(responseData.size());
            send(clientSocket, &responseLength, sizeof(responseLength), 0);
            send(clientSocket, responseData.data(), responseData.size(), 0);
        }
        
        close(clientSocket);
    }
};
