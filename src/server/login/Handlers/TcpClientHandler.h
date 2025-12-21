#pragma once

#include "../Interfaces/IClientHandler.h"
#include "../Interfaces/IMessageHandler.h"
#include "../Interfaces/IEncryptionService.h"
#include "../Interfaces/IInputValidator.h"
#include <memory>
#include <vector>
#include <sys/socket.h>
#include <unistd.h>
#include <cstring>
#include <iostream>
#include <arpa/inet.h>

namespace CloneMine {
namespace Login {
namespace Handlers {

/**
 * @brief TCP client handler
 * Handles individual client connections with encryption support.
 */
class TcpClientHandler : public IClientHandler {
private:
    int clientSocket_;
    std::shared_ptr<IEncryptionService> encryptionService_;
    std::shared_ptr<IMessageHandler> messageHandler_;
    std::shared_ptr<IInputValidator> inputValidator_;

    /**
     * @brief Read exact number of bytes from socket
     */
    bool ReadExact(std::vector<unsigned char>& buffer, size_t size) {
        buffer.resize(size);
        size_t totalRead = 0;
        
        while (totalRead < size) {
            ssize_t bytesRead = recv(clientSocket_, 
                                    buffer.data() + totalRead, 
                                    size - totalRead, 
                                    0);
            
            if (bytesRead <= 0) {
                return false;
            }
            
            totalRead += bytesRead;
        }
        
        return true;
    }

    /**
     * @brief Read message with length prefix
     * Format: [4 bytes length][message data]
     */
    bool ReadMessage(std::vector<unsigned char>& message) {
        // Read length prefix (4 bytes, little-endian)
        std::vector<unsigned char> lengthBytes;
        if (!ReadExact(lengthBytes, 4)) {
            return false;
        }

        uint32_t length = *reinterpret_cast<uint32_t*>(lengthBytes.data());
        
        // Validate message length
        auto validation = inputValidator_->ValidateMessageLength(length);
        if (!validation.first) {
            std::cerr << "Invalid message length: " << validation.second << std::endl;
            return false;
        }

        // Read message data
        return ReadExact(message, length);
    }

    /**
     * @brief Write message with length prefix
     * Format: [4 bytes length][message data]
     */
    bool WriteMessage(const std::vector<unsigned char>& message) {
        // Write length prefix (4 bytes, little-endian)
        uint32_t length = static_cast<uint32_t>(message.size());
        const unsigned char* lengthBytes = reinterpret_cast<const unsigned char*>(&length);
        
        if (send(clientSocket_, lengthBytes, 4, 0) != 4) {
            return false;
        }

        // Write message data
        size_t totalSent = 0;
        while (totalSent < message.size()) {
            ssize_t bytesSent = send(clientSocket_, 
                                    message.data() + totalSent, 
                                    message.size() - totalSent, 
                                    0);
            
            if (bytesSent <= 0) {
                return false;
            }
            
            totalSent += bytesSent;
        }
        
        return true;
    }

public:
    TcpClientHandler(int clientSocket,
                    std::shared_ptr<IEncryptionService> encryptionService,
                    std::shared_ptr<IMessageHandler> messageHandler,
                    std::shared_ptr<IInputValidator> inputValidator)
        : clientSocket_(clientSocket),
          encryptionService_(encryptionService),
          messageHandler_(messageHandler),
          inputValidator_(inputValidator) {
    }

    ~TcpClientHandler() {
        if (clientSocket_ >= 0) {
            close(clientSocket_);
        }
    }

    void Handle() override {
        try {
            while (true) {
                // Read incoming message
                std::vector<unsigned char> messageData;
                if (!ReadMessage(messageData)) {
                    break;
                }

                // Try to decrypt (fallback to plaintext if encryption fails)
                std::vector<unsigned char> decryptedData;
                std::string messageText;
                
                if (encryptionService_->TryDecrypt(messageData, decryptedData)) {
                    messageText = std::string(decryptedData.begin(), decryptedData.end());
                }
                else {
                    // Fallback to plaintext for testing
                    messageText = std::string(messageData.begin(), messageData.end());
                }

                std::cout << "Received: " << messageText << std::endl;

                // Handle message
                std::string response = messageHandler_->HandleMessage(messageText);
                
                std::cout << "Sending: " << response << std::endl;

                // Encrypt response
                std::vector<unsigned char> responseBytes(response.begin(), response.end());
                std::vector<unsigned char> encryptedResponse;
                
                try {
                    encryptedResponse = encryptionService_->Encrypt(responseBytes);
                }
                catch (...) {
                    // Fallback to plaintext if encryption fails
                    encryptedResponse = responseBytes;
                }

                // Send response
                if (!WriteMessage(encryptedResponse)) {
                    break;
                }
            }
        }
        catch (const std::exception& e) {
            std::cerr << "Client handler exception: " << e.what() << std::endl;
        }

        std::cout << "Client disconnected" << std::endl;
    }
};

} // namespace Handlers
} // namespace Login
} // namespace CloneMine
