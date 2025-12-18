#pragma once

#include <string>
#include <vector>
#include <map>
#include <cstdint>
#include "../combat/DamageTypes.h"

namespace clonemine {
namespace character {

// =============================================================================
// Archetype - The class category that determines primary/secondary set rules
// =============================================================================
enum class ClassArchetype {
    ATTACKER,       // Primary: Attack set, Secondary: Attack or Heal if class fits
    TANK,           // Primary: Defense/Protection set, Secondary: Attack set
    HEALER,         // Primary: Healing set, Secondary: Attack set (reduced effectiveness)
    MASTERMIND      // Primary: Summon/Pet set, Secondary: Buff/Heal set
};

// =============================================================================
// Powerset Category - What type of abilities a powerset provides
// =============================================================================
enum class PowersetCategory {
    ATTACK,         // Damage dealing abilities
    DEFENSE,        // Damage mitigation, shields, protection
    HEALING,        // Health restoration
    BUFF,           // Stat boosts, enhancements
    SUMMON,         // Pet/minion summoning
    CONTROL         // Crowd control, debuffs
};

// =============================================================================
// Powerset Ability - A single ability within a powerset (loaded from Lua)
// =============================================================================
struct PowersetAbility {
    uint32_t id;
    std::string name;
    int level;           // Level required to learn
    float damage;        // Base damage (if applicable)
    float heal;          // Base heal (if applicable)
    float cooldown;      // Cooldown in seconds
    float duration;      // Duration for DoTs/HoTs/buffs
    bool isAoe;          // Is area of effect
    bool isDot;          // Is damage over time
    std::string effect;  // For control/buff effects
    
    PowersetAbility() : id(0), level(1), damage(0), heal(0), cooldown(0),
                        duration(0), isAoe(false), isDot(false) {}
};

// =============================================================================
// Powerset - A themed set of abilities from a magic school
// =============================================================================
struct Powerset {
    std::string name;
    std::string description;
    PowersetCategory category;
    ElementalDamageType damageType;  // Primary element (if applicable)
    bool isPrimary;                   // Is this a primary or secondary set?
    float effectivenessModifier;      // 1.0 for primary, 0.75 for secondary typically
    
    // List of abilities in this powerset (loaded from Lua)
    std::vector<PowersetAbility> abilities;
    
    // Legacy: ability IDs for compatibility
    std::vector<uint32_t> abilityIds;
    
    Powerset() : category(PowersetCategory::ATTACK), 
                 damageType(ElementalDamageType::NONE),
                 isPrimary(true), effectivenessModifier(1.0f) {}
    
    Powerset(const std::string& n, const std::string& desc, 
             PowersetCategory cat, ElementalDamageType elem)
        : name(n), description(desc), category(cat), damageType(elem),
          isPrimary(true), effectivenessModifier(1.0f) {}
};

// =============================================================================
// Character Class Definition - Defines a playable class with its archetypes
// =============================================================================
struct CharacterClass {
    std::string className;
    std::string description;
    ClassArchetype archetype;
    
    // Primary stat for this class
    std::string primaryStat;  // "strength", "intelligence", "wisdom", etc.
    
    // Resource type
    std::string resourceType; // "mana", "energy", "rage", "focus"
    
    // Allowed powerset names (loaded from Lua)
    std::vector<std::string> allowedPrimaryPowersetNames;
    std::vector<std::string> allowedSecondaryPowersetNames;
    
    // Available powersets for primary slot (resolved at runtime)
    std::vector<Powerset> availablePrimaryPowersets;
    
    // Available powersets for secondary slot (resolved at runtime)
    std::vector<Powerset> availableSecondaryPowersets;
    
    // Health and resource scaling
    float baseHealth;
    float healthPerLevel;
    float baseResource;
    float resourcePerLevel;
    
    // Secondary effectiveness for this class
    float secondaryEffectiveness;
    
    CharacterClass() : archetype(ClassArchetype::ATTACKER), 
                       baseHealth(100.0f), healthPerLevel(10.0f),
                       baseResource(100.0f), resourcePerLevel(5.0f),
                       secondaryEffectiveness(0.75f) {}
};

// =============================================================================
// Character Powerset Selection - Stores the player's chosen powersets
// =============================================================================
struct PowersetSelection {
    Powerset primaryPowerset;
    Powerset secondaryPowerset;
    
    // Secondary effectiveness (typically 0.75 or 0.8 depending on class)
    float secondaryEffectiveness;
    
    PowersetSelection() : secondaryEffectiveness(0.75f) {}
};

// =============================================================================
// ClassSystem - Manages all class definitions and powerset availability
// Loads all data from Lua files in data/powersets/
// =============================================================================
class ClassSystem {
public:
    ClassSystem();
    
    // Initialize all classes and powersets (loads from Lua)
    void initialize();
    
    // Reload data from Lua files
    void reload();
    
    // Get all available classes
    const std::vector<CharacterClass>& getAvailableClasses() const { return m_classes; }
    
    // Get a specific class by name
    const CharacterClass* getClass(const std::string& className) const;
    
    // Get available primary powersets for a class
    std::vector<Powerset> getAvailablePrimaryPowersets(const std::string& className) const;
    
    // Get available secondary powersets for a class
    std::vector<Powerset> getAvailableSecondaryPowersets(const std::string& className) const;
    
    // Validate if a powerset selection is valid for a class
    bool isValidPowersetSelection(const std::string& className, 
                                  const std::string& primaryPowerset,
                                  const std::string& secondaryPowerset) const;
    
    // Create a powerset selection for a character
    PowersetSelection createPowersetSelection(const std::string& className,
                                              const std::string& primaryPowerset,
                                              const std::string& secondaryPowerset) const;
    
    // Get all powersets by category
    std::vector<Powerset> getPowersetsByCategory(PowersetCategory category) const;
    
    // Get secondary effectiveness for an archetype
    float getSecondaryEffectiveness(ClassArchetype archetype) const;
    
    // Get the data directory path
    static std::string getDataPath() { return "data/powersets/"; }
    
private:
    std::vector<CharacterClass> m_classes;
    std::vector<Powerset> m_allPowersets;
    std::map<std::string, Powerset> m_powersetsByName;
    bool m_initialized;
    
    // Lua loading methods
    bool loadPowersetsFromLua(const std::string& filepath);
    bool loadClassesFromLua(const std::string& filepath);
    void resolveClassPowersets();
    
    // Helper to convert string to category
    PowersetCategory stringToCategory(const std::string& cat) const;
    
    // Helper to convert string to archetype
    ClassArchetype stringToArchetype(const std::string& arch) const;
    
    // Helper to convert string to damage type
    ElementalDamageType stringToDamageType(const std::string& type) const;
    
    // Find powerset by name
    const Powerset* findPowerset(const std::string& name) const;
};

// =============================================================================
// Archetype Helper Functions
// =============================================================================
inline std::string archetypeToString(ClassArchetype archetype) {
    switch (archetype) {
        case ClassArchetype::ATTACKER: return "Attacker";
        case ClassArchetype::TANK: return "Tank";
        case ClassArchetype::HEALER: return "Healer";
        case ClassArchetype::MASTERMIND: return "Mastermind";
    }
    return "Unknown";
}

inline std::string categoryToString(PowersetCategory category) {
    switch (category) {
        case PowersetCategory::ATTACK: return "Attack";
        case PowersetCategory::DEFENSE: return "Defense";
        case PowersetCategory::HEALING: return "Healing";
        case PowersetCategory::BUFF: return "Buff";
        case PowersetCategory::SUMMON: return "Summon";
        case PowersetCategory::CONTROL: return "Control";
    }
    return "Unknown";
}

} // namespace character
} // namespace clonemine
