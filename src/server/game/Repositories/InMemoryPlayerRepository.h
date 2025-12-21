#pragma once

#include "../Interfaces/IPlayerRepository.h"
#include "../Models/Player.h"
#include <map>
#include <mutex>
#include <memory>
#include <vector>

namespace CloneMine {
namespace Game {
namespace Repositories {

class InMemoryPlayerRepository : public Interfaces::IPlayerRepository {
private:
    std::map<std::string, std::shared_ptr<Models::Player>> players;
    mutable std::mutex mutex;

public:
    std::shared_ptr<Models::Player> GetPlayer(const std::string& playerId) override {
        std::lock_guard<std::mutex> lock(mutex);
        auto it = players.find(playerId);
        return (it != players.end()) ? it->second : nullptr;
    }
    
    std::vector<std::shared_ptr<Models::Player>> GetAllPlayers() override {
        std::lock_guard<std::mutex> lock(mutex);
        std::vector<std::shared_ptr<Models::Player>> result;
        for (const auto& pair : players) {
            result.push_back(pair.second);
        }
        return result;
    }
    
    bool AddPlayer(std::shared_ptr<Models::Player> player) override {
        if (!player || player->PlayerId.empty()) {
            return false;
        }
        
        std::lock_guard<std::mutex> lock(mutex);
        if (players.find(player->PlayerId) != players.end()) {
            return false;  // Player already exists
        }
        
        players[player->PlayerId] = player;
        return true;
    }
    
    bool UpdatePlayer(std::shared_ptr<Models::Player> player) override {
        if (!player || player->PlayerId.empty()) {
            return false;
        }
        
        std::lock_guard<std::mutex> lock(mutex);
        if (players.find(player->PlayerId) == players.end()) {
            return false;  // Player doesn't exist
        }
        
        players[player->PlayerId] = player;
        return true;
    }
    
    bool RemovePlayer(const std::string& playerId) override {
        std::lock_guard<std::mutex> lock(mutex);
        return players.erase(playerId) > 0;
    }
    
    bool PlayerExists(const std::string& playerId) override {
        std::lock_guard<std::mutex> lock(mutex);
        return players.find(playerId) != players.end();
    }
    
    int GetPlayerCount() override {
        std::lock_guard<std::mutex> lock(mutex);
        return static_cast<int>(players.size());
    }
};

} // namespace Repositories
} // namespace Game
} // namespace CloneMine
