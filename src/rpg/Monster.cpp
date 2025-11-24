#include "Monster.h"
#include <cmath>
#include <algorithm>

namespace RPG {

Monster::Monster(const std::string& name, int baseLevel)
    : name_(name), level_(baseLevel), difficultyMultiplier_(1.0f) {
    // Initialize base stats
    stats_.health = 100 + (baseLevel * 15);
    stats_.damage = 10 + (baseLevel * 2);
    stats_.armor = 5 + (baseLevel * 1);
    stats_.level = baseLevel;
}

Monster Monster::scaleToLevel(int targetLevel) const {
    Monster scaled(name_, targetLevel);
    
    float scaleFactor = static_cast<float>(targetLevel) / level_;
    scaled.stats_.health = static_cast<int>(stats_.health * scaleFactor);
    scaled.stats_.damage = static_cast<int>(stats_.damage * scaleFactor);
    scaled.stats_.armor = static_cast<int>(stats_.armor * scaleFactor);
    scaled.difficultyMultiplier_ = difficultyMultiplier_;
    
    return scaled;
}

int Monster::calculateXPReward(int playerLevel) const {
    int levelDiff = level_ - playerLevel;
    int baseXP = 100 * level_;
    
    if (std::abs(levelDiff) > 5) {
        return levelDiff > 5 ? 0 : static_cast<int>(baseXP * 0.1f);
    }
    
    float multiplier = 1.0f;
    if (levelDiff > 0) {
        multiplier = 1.0f + (levelDiff * 0.15f);
    } else if (levelDiff < 0) {
        multiplier = std::max(0.2f, 1.0f + (levelDiff * 0.15f));
    }
    
    return static_cast<int>(baseXP * multiplier);
}

bool Monster::isInRange(int playerLevel) const {
    return std::abs(level_ - playerLevel) <= 5;
}

} // namespace RPG
