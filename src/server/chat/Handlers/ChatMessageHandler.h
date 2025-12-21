#pragma once

#include "../Interfaces/IMessageHandler.h"
#include "../Interfaces/IChatService.h"
#include "../Interfaces/IInputValidator.h"
#include <memory>
#include <string>
#include <sstream>

namespace CloneMine {
namespace Chat {

/**
 * @brief Message handler for chat protocol commands
 * 
 * Follows Single Responsibility Principle - only handles message routing
 * Follows Dependency Inversion Principle - depends on interfaces
 * 
 * Protocol:
 * - SEND <sender> <message> - Send a message
 * - GET_HISTORY [count] - Get chat history
 * - PING - Server health check
 */
class ChatMessageHandler : public IMessageHandler {
private:
    std::shared_ptr<IChatService> chatService;
    std::shared_ptr<IInputValidator> validator;

public:
    ChatMessageHandler(std::shared_ptr<IChatService> chatService,
                       std::shared_ptr<IInputValidator> validator)
        : chatService(chatService), validator(validator) {}

    std::string HandleMessage(const std::string& message) override {
        auto [valid, error] = validator->ValidateCommand(message);
        if (!valid) {
            return "ERROR " + error;
        }

        std::istringstream iss(message);
        std::string command;
        iss >> command;

        if (command == "SEND") {
            return HandleSend(iss);
        }
        else if (command == "GET_HISTORY") {
            return HandleGetHistory(iss);
        }
        else if (command == "PING") {
            return "PONG";
        }
        else {
            return "ERROR Unknown command";
        }
    }

    bool IsValidCommand(const std::string& command) const override {
        return command == "SEND" || command == "GET_HISTORY" || command == "PING";
    }

private:
    std::string HandleSend(std::istringstream& iss) {
        std::string sender;
        iss >> sender;

        if (sender.empty()) {
            return "ERROR Sender required";
        }

        // Get the rest as message content
        std::string content;
        std::getline(iss, content);
        if (!content.empty() && content[0] == ' ') {
            content = content.substr(1); // Remove leading space
        }

        if (content.empty()) {
            return "ERROR Message cannot be empty";
        }

        try {
            chatService->SendMessage(sender, content);
            return "SUCCESS Message sent";
        }
        catch (const std::exception& e) {
            return "ERROR " + std::string(e.what());
        }
    }

    std::string HandleGetHistory(std::istringstream& iss) {
        int count = 10; // Default
        iss >> count;

        if (count <= 0) {
            count = 10;
        }
        if (count > 100) {
            count = 100; // Max limit
        }

        try {
            return chatService->GetHistory(count);
        }
        catch (const std::exception& e) {
            return "ERROR " + std::string(e.what());
        }
    }
};

} // namespace Chat
} // namespace CloneMine
