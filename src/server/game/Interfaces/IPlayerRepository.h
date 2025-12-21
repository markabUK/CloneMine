#pragma once

#include "../Models/Player.h"
#include <vector>
#include <memory>

namespace CloneMine {
namespace Game {
namespace Interfaces {

class IPlayerRepository {
public:
    virtual ~IPlayerRepository() = default;
    
    virtual std::shared_ptr<Models::Player> GetPlayer(const std::string& playerId) = 0;
    virtual std::vector<std::shared_ptr<Models::Player>> GetAllPlayers() = 0;
    virtual bool AddPlayer(std::shared_ptr<Models::Player> player) = 0;
    virtual bool UpdatePlayer(std::shared_ptr<Models::Player> player) = 0;
    virtual bool RemovePlayer(const std::string& playerId) = 0;
    virtual bool PlayerExists(const std::string& playerId) = 0;
    virtual int GetPlayerCount() = 0;
};

} // namespace Interfaces
} // namespace Game
} // namespace CloneMine
