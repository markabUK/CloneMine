#ifndef QUEST_MESSAGE_HANDLER_H
#define QUEST_MESSAGE_HANDLER_H

#include <memory>
#include <sstream>
#include <vector>
#include "../Interfaces/IMessageHandler.h"
#include "../Interfaces/IQuestService.h"

namespace CloneMine {
namespace Quest {

class QuestMessageHandler : public IMessageHandler {
private:
    std::shared_ptr<IQuestService> questService;
    
    std::vector<std::string> Split(const std::string& str) {
        std::istringstream iss(str);
        std::vector<std::string> tokens;
        std::string token;
        while (iss >> token) {
            tokens.push_back(token);
        }
        return tokens;
    }
    
public:
    QuestMessageHandler(std::shared_ptr<IQuestService> questService)
        : questService(questService) {}
    
    std::string HandleMessage(const std::string& message) override {
        auto tokens = Split(message);
        if (tokens.empty()) return "ERROR Invalid command";
        
        std::string command = tokens[0];
        
        if (command == "LIST") {
            std::string typeFilter = tokens.size() > 1 ? tokens[1] : "ALL";
            return questService->ListQuests(typeFilter);
        }
        else if (command == "ACCEPT") {
            if (tokens.size() < 3) return "ERROR Usage: ACCEPT <playerId> <questId>";
            return questService->AcceptQuest(tokens[1], tokens[2]);
        }
        else if (command == "UPDATE") {
            if (tokens.size() < 4) return "ERROR Usage: UPDATE <playerId> <questId> <progress>";
            try {
                double progress = std::stod(tokens[3]);
                return questService->UpdateProgress(tokens[1], tokens[2], progress);
            } catch (...) {
                return "ERROR Invalid progress value";
            }
        }
        else if (command == "COMPLETE") {
            if (tokens.size() < 3) return "ERROR Usage: COMPLETE <playerId> <questId>";
            return questService->CompleteQuest(tokens[1], tokens[2]);
        }
        else if (command == "GET_PROGRESS") {
            if (tokens.size() < 2) return "ERROR Usage: GET_PROGRESS <playerId>";
            return questService->GetPlayerProgress(tokens[1]);
        }
        else if (command == "PING") {
            return "PONG";
        }
        
        return "ERROR Unknown command";
    }
};

} // namespace Quest
} // namespace CloneMine

#endif
