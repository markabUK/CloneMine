#pragma once

#include "../Models/Player.h"
#include <memory>
#include <string>

namespace CloneMine {
namespace Game {
namespace Interfaces {

class IGameService {
public:
    virtual ~IGameService() = default;
    
    virtual bool JoinGame(const std::string& playerId, const std::string& username) = 0;
    virtual bool LeaveGame(const std::string& playerId) = 0;
    virtual bool MovePlayer(const std::string& playerId, double x, double y, double z) = 0;
    virtual bool AttackPlayer(const std::string& attackerId, const std::string& targetId) = 0;
    virtual std::shared_ptr<Models::Player> GetPlayerState(const std::string& playerId) = 0;
    virtual void UpdateGameState() = 0;
    virtual void StartGameLoop() = 0;
    virtual void StopGameLoop() = 0;
};

} // namespace Interfaces
} // namespace Game
} // namespace CloneMine
