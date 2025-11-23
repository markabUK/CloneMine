#pragma once

#include "DamageTypes.h"
#include <cmath>

namespace clonemine {

class DamageCalculation {
public:
    static float calculateDamage(const DamageInfo& damage, const ResistanceProfile& resistance) {
        float totalDamage = 0.0f;
        
        for (const auto& component : damage.components) {
            float componentDamage = damage.baseDamage * component.percentage;
            float resist = 0.0f;
            
            if (component.elementalType != ElementalDamageType::NONE) {
                resist = resistance.getResistance(component.elementalType);
            } else {
                resist = resistance.getResistance(component.physicalType);
            }
            
            // Apply resistance: 1.0 resist = immune, -1.0 resist = 200% damage
            float damageMultiplier = 1.0f - resist;
            damageMultiplier = std::max(0.0f, std::min(3.0f, damageMultiplier)); // Cap at 0-300%
            
            totalDamage += componentDamage * damageMultiplier;
        }
        
        return totalDamage;
    }
    
    // Helper to create common damage types
    static DamageInfo createPhysicalDamage(float baseDamage, PhysicalDamageType type) {
        DamageInfo info(baseDamage);
        info.addComponent(DamageComponent(type, 1.0f));
        return info;
    }
    
    static DamageInfo createElementalDamage(float baseDamage, ElementalDamageType type) {
        DamageInfo info(baseDamage);
        info.addComponent(DamageComponent(type, 1.0f));
        return info;
    }
    
    static DamageInfo createMixedDamage(float baseDamage, PhysicalDamageType physType, 
                                       float physPct, ElementalDamageType elemType, float elemPct) {
        DamageInfo info(baseDamage);
        info.addComponent(DamageComponent(physType, physPct));
        info.addComponent(DamageComponent(elemType, elemPct));
        return info;
    }
};

} // namespace clonemine
