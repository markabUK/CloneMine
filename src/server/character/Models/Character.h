#pragma once
#include <string>

struct Character {
    std::string id;
    std::string name;
    int level;
    long long xp;
    int health;
    int mana;
    std::string characterClass;
    
    Character(const std::string& id, const std::string& name, const std::string& cls)
        : id(id), name(name), level(1), xp(0), health(100), mana(50), characterClass(cls) {}
    
    Character(const std::string& id, const std::string& name, int level, long long xp,
              int health, int mana, const std::string& cls)
        : id(id), name(name), level(level), xp(xp), health(health), mana(mana), characterClass(cls) {}
};
