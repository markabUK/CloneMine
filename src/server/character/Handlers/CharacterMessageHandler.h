#pragma once
#include "../Interfaces/IMessageHandler.h"
#include "../Interfaces/ICharacterService.h"
#include <memory>
#include <sstream>
#include <vector>

class CharacterMessageHandler : public IMessageHandler {
private:
    std::shared_ptr<ICharacterService> service;
    
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
    CharacterMessageHandler(std::shared_ptr<ICharacterService> svc)
        : service(svc) {}
    
    std::string HandleMessage(const std::string& message) override {
        auto parts = SplitString(message, ' ');
        if (parts.empty()) {
            return "ERROR Invalid command";
        }
        
        std::string command = parts[0];
        
        if (command == "CREATE") {
            if (parts.size() < 3) {
                return "ERROR CREATE requires <name> <class>";
            }
            return service->CreateCharacter(parts[1], parts[2]);
        }
        else if (command == "GET") {
            if (parts.size() < 2) {
                return "ERROR GET requires <characterId>";
            }
            return service->GetCharacter(parts[1]);
        }
        else if (command == "UPDATE") {
            if (parts.size() < 6) {
                return "ERROR UPDATE requires <characterId> <level> <xp> <health> <mana>";
            }
            try {
                int level = std::stoi(parts[2]);
                long long xp = std::stoll(parts[3]);
                int health = std::stoi(parts[4]);
                int mana = std::stoi(parts[5]);
                return service->UpdateCharacter(parts[1], level, xp, health, mana);
            } catch (const std::exception& e) {
                return "ERROR Invalid numeric values";
            }
        }
        else if (command == "DELETE") {
            if (parts.size() < 2) {
                return "ERROR DELETE requires <characterId>";
            }
            return service->DeleteCharacter(parts[1]);
        }
        else if (command == "LIST") {
            return service->ListCharacters();
        }
        else if (command == "PING") {
            return "PONG";
        }
        
        return "ERROR Unknown command";
    }
};
