#pragma once

#include <string>
#include <vector>
#include <memory>

namespace RPG {

struct MonsterStats {
    int health;
    int damage;
    int armor;
    int level;
};

class Monster {
public:
    Monster(const std::string& name, int baseLevel);
    
    std::string getName() const { return name_; }
    int getLevel() const { return level_; }
    MonsterStats getStats() const { return stats_; }
    
    // City of Heroes style scaling
    Monster scaleToLevel(int targetLevel) const;
    int calculateXPReward(int playerLevel) const;
    bool isInRange(int playerLevel) const;
    
private:
    std::string name_;
    int level_;
    MonsterStats stats_;
    float difficultyMultiplier_;
};

} // namespace RPG
