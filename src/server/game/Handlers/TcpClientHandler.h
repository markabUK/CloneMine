#pragma once

#include "../Interfaces/IClientHandler.h"
#include "../Interfaces/IMessageHandler.h"
#include "../Interfaces/IEncryptionService.h"
#include <memory>
#include <string>
#include <vector>
#include <cstring>
#include <sstream>
#include <iomanip>
#include <cstdlib>

#ifdef _WIN32
#include <winsock2.h>
#else
#include <sys/socket.h>
#include <unistd.h>
#endif

namespace CloneMine {
namespace Game {
namespace Handlers {

class TcpClientHandler : public Interfaces::IClientHandler {
private:
    std::shared_ptr<Interfaces::IMessageHandler> messageHandler;
    std::shared_ptr<Interfaces::IEncryptionService> encryptionService;
    int socket;
    std::string clientId;

    std::string GenerateClientId() {
        std::stringstream ss;
        ss << std::hex << std::setfill('0');
        for (int i = 0; i < 16; ++i) {
            ss << std::setw(2) << (rand() % 256);
        }
        return ss.str();
    }

public:
    TcpClientHandler(std::shared_ptr<Interfaces::IMessageHandler> handler,
                    std::shared_ptr<Interfaces::IEncryptionService> encryption)
        : messageHandler(handler),
          encryptionService(encryption),
          socket(-1),
          clientId(GenerateClientId()) {
    }

    void HandleClient(int clientSocket) override {
        socket = clientSocket;

        while (true) {
            std::string message = ReceiveMessage();
            if (message.empty()) {
                break;  // Connection closed
            }

            std::string response = messageHandler->HandleMessage(message, clientId);
            SendMessage(response);

            if (message == "LEAVE") {
                break;
            }
        }

        CloseConnection();
    }

    void SendMessage(const std::string& message) override {
        if (socket == -1) return;

        std::vector<unsigned char> data(message.begin(), message.end());
        
        // Try encryption, fallback to plaintext
        if (encryptionService->IsEnabled()) {
            try {
                data = encryptionService->Encrypt(data);
            } catch (...) {
                // Encryption failed, send plaintext
            }
        }

        // Send length prefix (4 bytes)
        uint32_t length = static_cast<uint32_t>(data.size());
        send(socket, reinterpret_cast<char*>(&length), 4, 0);
        
        // Send data
        send(socket, reinterpret_cast<char*>(data.data()), data.size(), 0);
    }

    std::string ReceiveMessage() override {
        if (socket == -1) return "";

        // Receive length prefix
        uint32_t length = 0;
        int received = recv(socket, reinterpret_cast<char*>(&length), 4, 0);
        if (received <= 0 || length == 0 || length > 1048576) {  // Max 1MB
            return "";
        }

        // Receive data
        std::vector<unsigned char> data(length);
        received = recv(socket, reinterpret_cast<char*>(data.data()), length, 0);
        if (received <= 0) {
            return "";
        }

        // Try decryption, fallback to plaintext
        if (encryptionService->IsEnabled()) {
            try {
                data = encryptionService->Decrypt(data);
            } catch (...) {
                // Decryption failed, assume plaintext
            }
        }

        return std::string(data.begin(), data.end());
    }

    void CloseConnection() override {
        if (socket != -1) {
#ifdef _WIN32
            closesocket(socket);
#else
            close(socket);
#endif
            socket = -1;
        }
    }
};

} // namespace Handlers
} // namespace Game
} // namespace CloneMine
