#include "CharacterClass.h"
#include <algorithm>

namespace RPG {

bool CharacterClass::canUseEquipment(const std::string& equipmentType) const {
    // If no restrictions, can use anything
    if (equipmentRestrictions.empty()) {
        return true;
    }
    
    // Check if equipment type is in restrictions (whitelist)
    return std::find(equipmentRestrictions.begin(), 
                    equipmentRestrictions.end(), 
                    equipmentType) != equipmentRestrictions.end();
}

float CharacterClass::calculateDamageMultiplier(const std::string& damageType, 
                                                const Stats& currentStats) const {
    float multiplier = 1.0f;
    
    for (const auto& scaling : damageScaling) {
        if (scaling.damageType == damageType) {
            int statValue = 10; // Default
            
            if (scaling.stat == "STR") statValue = currentStats.strength;
            else if (scaling.stat == "DEX") statValue = currentStats.dexterity;
            else if (scaling.stat == "CON") statValue = currentStats.constitution;
            else if (scaling.stat == "INT") statValue = currentStats.intelligence;
            else if (scaling.stat == "WIS") statValue = currentStats.wisdom;
            else if (scaling.stat == "CHA") statValue = currentStats.charisma;
            
            multiplier += (statValue * scaling.scalingFactor);
        }
    }
    
    return multiplier;
}

} // namespace RPG
