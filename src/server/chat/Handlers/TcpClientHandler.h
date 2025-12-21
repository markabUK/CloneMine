#pragma once

#include "../Interfaces/IClientHandler.h"
#include "../Interfaces/IMessageHandler.h"
#include "../Interfaces/IEncryptionService.h"
#include <sys/socket.h>
#include <unistd.h>
#include <memory>
#include <string>
#include <vector>

namespace CloneMine {
namespace Chat {

/**
 * @brief TCP client handler with encryption support
 * 
 * Follows Single Responsibility Principle - handles single client connection
 * Follows Dependency Inversion Principle - depends on interfaces
 */
class TcpClientHandler : public IClientHandler {
private:
    int clientSocket;
    std::shared_ptr<IMessageHandler> messageHandler;
    std::shared_ptr<IEncryptionService> encryption;
    bool connected;
    bool useEncryption;

public:
    TcpClientHandler(std::shared_ptr<IMessageHandler> messageHandler,
                     std::shared_ptr<IEncryptionService> encryption,
                     bool useEncryption = true)
        : clientSocket(-1),
          messageHandler(messageHandler),
          encryption(encryption),
          connected(false),
          useEncryption(useEncryption) {}

    ~TcpClientHandler() {
        Disconnect();
    }

    void HandleClient(int socket) override {
        clientSocket = socket;
        connected = true;

        while (connected) {
            // Read message length (4 bytes)
            uint32_t messageLength;
            ssize_t bytesRead = recv(clientSocket, &messageLength, sizeof(messageLength), 0);
            
            if (bytesRead <= 0) {
                break;
            }

            messageLength = ntohl(messageLength); // Network to host byte order

            if (messageLength == 0 || messageLength > 10240) { // Max 10KB
                break;
            }

            // Read message data
            std::vector<unsigned char> buffer(messageLength);
            bytesRead = recv(clientSocket, buffer.data(), messageLength, 0);

            if (bytesRead <= 0) {
                break;
            }

            // Decrypt if encryption is enabled
            std::string message;
            if (useEncryption) {
                try {
                    message = encryption->Decrypt(buffer);
                }
                catch (...) {
                    // Try as plaintext if decryption fails
                    message = std::string(buffer.begin(), buffer.end());
                }
            }
            else {
                message = std::string(buffer.begin(), buffer.end());
            }

            // Handle message and get response
            std::string response = messageHandler->HandleMessage(message);

            // Send response
            SendMessage(response);
        }

        Disconnect();
    }

    void SendMessage(const std::string& message) override {
        if (!connected || clientSocket < 0) {
            return;
        }

        std::vector<unsigned char> data;

        // Encrypt if encryption is enabled
        if (useEncryption) {
            try {
                data = encryption->Encrypt(message);
            }
            catch (...) {
                // Fallback to plaintext
                data = std::vector<unsigned char>(message.begin(), message.end());
            }
        }
        else {
            data = std::vector<unsigned char>(message.begin(), message.end());
        }

        // Send length prefix
        uint32_t length = htonl(static_cast<uint32_t>(data.size()));
        send(clientSocket, &length, sizeof(length), 0);

        // Send data
        send(clientSocket, data.data(), data.size(), 0);
    }

    void Disconnect() override {
        connected = false;
        if (clientSocket >= 0) {
            close(clientSocket);
            clientSocket = -1;
        }
    }

    bool IsConnected() const override {
        return connected;
    }
};

} // namespace Chat
} // namespace CloneMine
