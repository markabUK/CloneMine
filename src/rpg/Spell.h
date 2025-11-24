#pragma once

#include <string>
#include <vector>

namespace RPG {

struct SpellTint {
    int r = 255;
    int g = 255;
    int b = 255;
};

class Spell {
public:
    Spell() = default;
    
    // Getters
    const std::string& getName() const { return name; }
    const std::string& getSchool() const { return school; }
    int getBaseDamage() const { return baseDamage; }
    int getManaCost() const { return manaCost; }
    float getCastTime() const { return castTime; }
    float getCooldown() const { return cooldown; }
    const SpellTint& getTint() const { return tint; }
    const std::vector<std::string>& getClasses() const { return classes; }
    
    // Setters
    void setName(const std::string& n) { name = n; }
    void setSchool(const std::string& s) { school = s; }
    void setBaseDamage(int damage) { baseDamage = damage; }
    void setManaCost(int cost) { manaCost = cost; }
    void setCastTime(float time) { castTime = time; }
    void setCooldown(float cd) { cooldown = cd; }
    void setTint(const SpellTint& t) { tint = t; }
    void addAllowedClass(const std::string& className) { classes.push_back(className); }
    
    // Utility
    bool canBeUsedBy(const std::string& className) const;
    
private:
    std::string name;
    std::string school;
    int baseDamage = 0;
    int manaCost = 0;
    float castTime = 0.0f;
    float cooldown = 0.0f;
    SpellTint tint;
    std::vector<std::string> classes;
};

} // namespace RPG
