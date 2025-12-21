#pragma once

#include "../Interfaces/IChatService.h"
#include "../Interfaces/IChatRepository.h"
#include "../Interfaces/IInputValidator.h"
#include "../Models/ChatMessage.h"
#include <memory>
#include <string>
#include <sstream>
#include <vector>
#include <functional>

namespace CloneMine {
namespace Chat {

/**
 * @brief Chat service implementation with broadcasting and history
 * 
 * Follows Single Responsibility Principle - manages chat business logic
 * Follows Dependency Inversion Principle - depends on interfaces
 */
class ChatService : public IChatService {
private:
    std::shared_ptr<IChatRepository> repository;
    std::shared_ptr<IInputValidator> validator;
    std::vector<std::function<void(const std::string&)>> broadcastCallbacks;
    bool running;

public:
    ChatService(std::shared_ptr<IChatRepository> repository,
                std::shared_ptr<IInputValidator> validator)
        : repository(repository), validator(validator), running(false) {}

    void SendMessage(const std::string& sender, const std::string& content) override {
        // Validate sender
        auto [senderValid, senderError] = validator->ValidateSender(sender);
        if (!senderValid) {
            throw std::runtime_error("Invalid sender: " + senderError);
        }

        // Validate message
        auto [messageValid, messageError] = validator->ValidateMessage(content);
        if (!messageValid) {
            throw std::runtime_error("Invalid message: " + messageError);
        }

        // Create and store message
        ChatMessage message(sender, content, std::time(nullptr));
        repository->AddMessage(message);

        // Broadcast to all connected clients
        BroadcastMessage(sender, content);
    }

    void BroadcastMessage(const std::string& sender, const std::string& content) override {
        std::ostringstream oss;
        oss << "MESSAGE " << sender << " " << content << " " << std::time(nullptr);
        std::string broadcast = oss.str();

        // Call all registered broadcast callbacks
        for (auto& callback : broadcastCallbacks) {
            callback(broadcast);
        }
    }

    std::string GetHistory(int count) override {
        auto messages = repository->GetHistory(count);
        
        if (messages.empty()) {
            return "HISTORY 0";
        }

        std::ostringstream oss;
        oss << "HISTORY " << messages.size();
        
        for (const auto& msg : messages) {
            oss << "\nMESSAGE " << msg.sender << " " << msg.content << " " << msg.timestamp;
        }

        return oss.str();
    }

    void RegisterBroadcastCallback(std::function<void(const std::string&)> callback) {
        broadcastCallbacks.push_back(callback);
    }

    void Start() override {
        running = true;
    }

    void Stop() override {
        running = false;
    }

    bool IsRunning() const {
        return running;
    }
};

} // namespace Chat
} // namespace CloneMine
