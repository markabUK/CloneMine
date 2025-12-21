#pragma once

#include "../Interfaces/IMessageHandler.h"
#include "../Interfaces/IGameService.h"
#include <memory>
#include <sstream>
#include <vector>

namespace CloneMine {
namespace Game {
namespace Handlers {

class GameMessageHandler : public Interfaces::IMessageHandler {
private:
    std::shared_ptr<Interfaces::IGameService> gameService;

    std::vector<std::string> SplitString(const std::string& str, char delimiter) {
        std::vector<std::string> tokens;
        std::stringstream ss(str);
        std::string token;
        while (std::getline(ss, token, delimiter)) {
            tokens.push_back(token);
        }
        return tokens;
    }

public:
    GameMessageHandler(std::shared_ptr<Interfaces::IGameService> service)
        : gameService(service) {
    }

    std::string HandleMessage(const std::string& message, const std::string& clientId) override {
        auto parts = SplitString(message, ' ');
        if (parts.empty()) {
            return "ERROR Invalid command";
        }

        std::string command = parts[0];

        if (command == "JOIN") {
            if (parts.size() < 2) {
                return "ERROR Usage: JOIN <username>";
            }
            std::string username = parts[1];
            bool success = gameService->JoinGame(clientId, username);
            if (success) {
                return "SUCCESS Joined game as " + username;
            } else {
                return "ERROR Failed to join game";
            }
        }
        else if (command == "LEAVE") {
            bool success = gameService->LeaveGame(clientId);
            if (success) {
                return "SUCCESS Left game";
            } else {
                return "ERROR Failed to leave game";
            }
        }
        else if (command == "MOVE") {
            if (parts.size() < 4) {
                return "ERROR Usage: MOVE <x> <y> <z>";
            }
            try {
                double x = std::stod(parts[1]);
                double y = std::stod(parts[2]);
                double z = std::stod(parts[3]);
                
                bool success = gameService->MovePlayer(clientId, x, y, z);
                if (success) {
                    std::stringstream ss;
                    ss << "SUCCESS Player moved to (" << x << ", " << y << ", " << z << ")";
                    return ss.str();
                } else {
                    return "ERROR Failed to move player";
                }
            } catch (...) {
                return "ERROR Invalid coordinates";
            }
        }
        else if (command == "ATTACK") {
            if (parts.size() < 2) {
                return "ERROR Usage: ATTACK <targetPlayerId>";
            }
            std::string targetId = parts[1];
            bool success = gameService->AttackPlayer(clientId, targetId);
            if (success) {
                return "SUCCESS Attack successful";
            } else {
                return "ERROR Attack failed";
            }
        }
        else if (command == "GET_STATE") {
            auto player = gameService->GetPlayerState(clientId);
            if (player) {
                std::stringstream ss;
                ss << "SUCCESS " << player->PlayerId << " "
                   << player->PositionX << " " << player->PositionY << " " << player->PositionZ << " "
                   << player->Health << " " << player->State;
                return ss.str();
            } else {
                return "ERROR Player not found";
            }
        }
        else if (command == "PING") {
            return "PONG";
        }
        else {
            return "ERROR Unknown command: " + command;
        }
    }
};

} // namespace Handlers
} // namespace Game
} // namespace CloneMine
