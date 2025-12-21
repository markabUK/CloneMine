#pragma once

#include <string>
#include <ctime>

namespace CloneMine {
namespace Game {
namespace Models {

struct Player {
    std::string PlayerId;
    std::string Username;
    double PositionX;
    double PositionY;
    double PositionZ;
    int Health;
    int State;  // 0 = idle, 1 = moving, 2 = attacking, 3 = dead
    std::time_t LastUpdate;
    
    Player()
        : PlayerId(""),
          Username(""),
          PositionX(0.0),
          PositionY(0.0),
          PositionZ(0.0),
          Health(100),
          State(0),
          LastUpdate(std::time(nullptr)) {
    }
    
    Player(const std::string& playerId, const std::string& username)
        : PlayerId(playerId),
          Username(username),
          PositionX(0.0),
          PositionY(0.0),
          PositionZ(0.0),
          Health(100),
          State(0),
          LastUpdate(std::time(nullptr)) {
    }
    
    Player(const std::string& playerId, const std::string& username, 
           double x, double y, double z)
        : PlayerId(playerId),
          Username(username),
          PositionX(x),
          PositionY(y),
          PositionZ(z),
          Health(100),
          State(0),
          LastUpdate(std::time(nullptr)) {
    }
    
    bool IsAlive() const {
        return Health > 0 && State != 3;
    }
    
    void UpdatePosition(double x, double y, double z) {
        PositionX = x;
        PositionY = y;
        PositionZ = z;
        LastUpdate = std::time(nullptr);
    }
    
    void TakeDamage(int damage) {
        Health -= damage;
        if (Health <= 0) {
            Health = 0;
            State = 3;  // Dead
        }
        LastUpdate = std::time(nullptr);
    }
    
    void Heal(int amount) {
        if (State != 3) {  // Can't heal if dead
            Health += amount;
            if (Health > 100) {
                Health = 100;
            }
            LastUpdate = std::time(nullptr);
        }
    }
};

} // namespace Models
} // namespace Game
} // namespace CloneMine
