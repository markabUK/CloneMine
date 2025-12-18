#include "ClassSystem.h"
#include <algorithm>
#include <iostream>
#include <fstream>
#include <sstream>

namespace clonemine {
namespace character {

ClassSystem::ClassSystem() : m_initialized(false) {
}

void ClassSystem::initialize() {
    std::cout << "[ClassSystem] Initializing class system from Lua files..." << std::endl;
    
    // Clear existing data
    m_classes.clear();
    m_allPowersets.clear();
    m_powersetsByName.clear();
    
    // Load powersets from Lua files
    std::vector<std::string> powersetFiles = {
        getDataPath() + "attack_powersets.lua",
        getDataPath() + "defense_powersets.lua",
        getDataPath() + "healing_powersets.lua",
        getDataPath() + "buff_powersets.lua",
        getDataPath() + "summon_powersets.lua",
        getDataPath() + "control_powersets.lua"
    };
    
    for (const auto& filepath : powersetFiles) {
        if (loadPowersetsFromLua(filepath)) {
            std::cout << "[ClassSystem] Loaded powersets from: " << filepath << std::endl;
        } else {
            std::cout << "[ClassSystem] Warning: Could not load " << filepath << std::endl;
        }
    }
    
    // Load class definitions from Lua
    if (loadClassesFromLua(getDataPath() + "classes.lua")) {
        std::cout << "[ClassSystem] Loaded class definitions from Lua" << std::endl;
    } else {
        std::cout << "[ClassSystem] Warning: Could not load class definitions" << std::endl;
    }
    
    // Resolve class powersets (link powerset names to actual powerset objects)
    resolveClassPowersets();
    
    m_initialized = true;
    
    std::cout << "[ClassSystem] Initialized " << m_classes.size() << " classes with " 
              << m_allPowersets.size() << " powersets (loaded from Lua)" << std::endl;
}

void ClassSystem::reload() {
    std::cout << "[ClassSystem] Reloading class system from Lua files..." << std::endl;
    initialize();
}

// =============================================================================
// Lua File Loading
// =============================================================================

// Simple Lua table parser (reads the return table from our Lua files)
// This is a lightweight parser that handles our specific Lua format
bool ClassSystem::loadPowersetsFromLua(const std::string& filepath) {
    std::ifstream file(filepath);
    if (!file.is_open()) {
        return false;
    }
    
    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string content = buffer.str();
    file.close();
    
    // Parse the Lua file for powerset definitions
    // Look for patterns like: { name = "...", description = "...", category = "...", ... }
    
    size_t pos = 0;
    while ((pos = content.find("name = \"", pos)) != std::string::npos) {
        Powerset powerset;
        
        // Extract name
        size_t nameStart = pos + 8;  // After 'name = "'
        size_t nameEnd = content.find("\"", nameStart);
        if (nameEnd != std::string::npos) {
            powerset.name = content.substr(nameStart, nameEnd - nameStart);
        }
        
        // Find the block for this powerset (between { and })
        size_t blockStart = content.rfind("{", pos);
        size_t blockEnd = content.find("},", pos);
        if (blockEnd == std::string::npos) {
            blockEnd = content.find("}", pos);
        }
        
        if (blockStart != std::string::npos && blockEnd != std::string::npos) {
            std::string block = content.substr(blockStart, blockEnd - blockStart);
            
            // Extract description
            size_t descPos = block.find("description = \"");
            if (descPos != std::string::npos) {
                size_t descStart = descPos + 15;
                size_t descEnd = block.find("\"", descStart);
                if (descEnd != std::string::npos) {
                    powerset.description = block.substr(descStart, descEnd - descStart);
                }
            }
            
            // Extract category
            size_t catPos = block.find("category = \"");
            if (catPos != std::string::npos) {
                size_t catStart = catPos + 12;
                size_t catEnd = block.find("\"", catStart);
                if (catEnd != std::string::npos) {
                    std::string catStr = block.substr(catStart, catEnd - catStart);
                    powerset.category = stringToCategory(catStr);
                }
            }
            
            // Extract damageType
            size_t dmgPos = block.find("damageType = \"");
            if (dmgPos != std::string::npos) {
                size_t dmgStart = dmgPos + 14;
                size_t dmgEnd = block.find("\"", dmgStart);
                if (dmgEnd != std::string::npos) {
                    std::string dmgStr = block.substr(dmgStart, dmgEnd - dmgStart);
                    powerset.damageType = stringToDamageType(dmgStr);
                }
            }
            
            // Parse abilities section
            size_t abilitiesPos = block.find("abilities = {");
            if (abilitiesPos != std::string::npos) {
                size_t abilitiesEnd = block.find("}", abilitiesPos + 13);
                // Find all abilities in this section
                size_t abilityPos = abilitiesPos;
                while ((abilityPos = block.find("{ id = ", abilityPos)) != std::string::npos && abilityPos < abilitiesEnd) {
                    PowersetAbility ability;
                    
                    // Extract ability ID
                    size_t idStart = abilityPos + 7;
                    size_t idEnd = block.find(",", idStart);
                    if (idEnd != std::string::npos) {
                        ability.id = std::stoul(block.substr(idStart, idEnd - idStart));
                    }
                    
                    // Find ability block end
                    size_t abilBlockEnd = block.find("}", abilityPos);
                    if (abilBlockEnd != std::string::npos) {
                        std::string abilBlock = block.substr(abilityPos, abilBlockEnd - abilityPos);
                        
                        // Extract ability name
                        size_t aNamePos = abilBlock.find("name = \"");
                        if (aNamePos != std::string::npos) {
                            size_t aNameStart = aNamePos + 8;
                            size_t aNameEnd = abilBlock.find("\"", aNameStart);
                            if (aNameEnd != std::string::npos) {
                                ability.name = abilBlock.substr(aNameStart, aNameEnd - aNameStart);
                            }
                        }
                        
                        // Extract level
                        size_t lvlPos = abilBlock.find("level = ");
                        if (lvlPos != std::string::npos) {
                            ability.level = std::stoi(abilBlock.substr(lvlPos + 8));
                        }
                        
                        // Extract damage
                        size_t dmgAbilPos = abilBlock.find("damage = ");
                        if (dmgAbilPos != std::string::npos) {
                            ability.damage = std::stof(abilBlock.substr(dmgAbilPos + 9));
                        }
                        
                        // Extract heal
                        size_t healPos = abilBlock.find("heal = ");
                        if (healPos != std::string::npos) {
                            ability.heal = std::stof(abilBlock.substr(healPos + 7));
                        }
                        
                        // Extract cooldown
                        size_t cdPos = abilBlock.find("cooldown = ");
                        if (cdPos != std::string::npos) {
                            ability.cooldown = std::stof(abilBlock.substr(cdPos + 11));
                        }
                        
                        // Extract isAoe
                        if (abilBlock.find("isAoe = true") != std::string::npos) {
                            ability.isAoe = true;
                        }
                        
                        // Extract isDot
                        if (abilBlock.find("isDot = true") != std::string::npos) {
                            ability.isDot = true;
                        }
                        
                        powerset.abilities.push_back(ability);
                        powerset.abilityIds.push_back(ability.id);
                    }
                    
                    abilityPos = abilBlockEnd;
                }
            }
        }
        
        // Add powerset if valid
        if (!powerset.name.empty()) {
            m_allPowersets.push_back(powerset);
            m_powersetsByName[powerset.name] = powerset;
            std::cout << "[ClassSystem]   Loaded powerset: " << powerset.name 
                      << " (" << categoryToString(powerset.category) << ", "
                      << powerset.abilities.size() << " abilities)" << std::endl;
        }
        
        pos = nameEnd + 1;
    }
    
    return true;
}

bool ClassSystem::loadClassesFromLua(const std::string& filepath) {
    std::ifstream file(filepath);
    if (!file.is_open()) {
        return false;
    }
    
    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string content = buffer.str();
    file.close();
    
    // Parse class definitions
    size_t pos = 0;
    while ((pos = content.find("name = \"", pos)) != std::string::npos) {
        CharacterClass charClass;
        
        // Extract name
        size_t nameStart = pos + 8;
        size_t nameEnd = content.find("\"", nameStart);
        if (nameEnd != std::string::npos) {
            charClass.className = content.substr(nameStart, nameEnd - nameStart);
        }
        
        // Find block for this class
        size_t blockStart = content.rfind("{", pos);
        size_t blockEnd = content.find("},", pos);
        if (blockEnd == std::string::npos) {
            blockEnd = content.find("}", pos);
        }
        
        if (blockStart != std::string::npos && blockEnd != std::string::npos) {
            std::string block = content.substr(blockStart, blockEnd - blockStart + 1);
            
            // Extract description
            size_t descPos = block.find("description = \"");
            if (descPos != std::string::npos) {
                size_t descStart = descPos + 15;
                size_t descEnd = block.find("\"", descStart);
                if (descEnd != std::string::npos) {
                    charClass.description = block.substr(descStart, descEnd - descStart);
                }
            }
            
            // Extract archetype
            size_t archPos = block.find("archetype = \"");
            if (archPos != std::string::npos) {
                size_t archStart = archPos + 13;
                size_t archEnd = block.find("\"", archStart);
                if (archEnd != std::string::npos) {
                    std::string archStr = block.substr(archStart, archEnd - archStart);
                    charClass.archetype = stringToArchetype(archStr);
                }
            }
            
            // Extract primaryStat
            size_t statPos = block.find("primaryStat = \"");
            if (statPos != std::string::npos) {
                size_t statStart = statPos + 15;
                size_t statEnd = block.find("\"", statStart);
                if (statEnd != std::string::npos) {
                    charClass.primaryStat = block.substr(statStart, statEnd - statStart);
                }
            }
            
            // Extract resourceType
            size_t resPos = block.find("resourceType = \"");
            if (resPos != std::string::npos) {
                size_t resStart = resPos + 16;
                size_t resEnd = block.find("\"", resStart);
                if (resEnd != std::string::npos) {
                    charClass.resourceType = block.substr(resStart, resEnd - resStart);
                }
            }
            
            // Extract numeric values
            size_t bhPos = block.find("baseHealth = ");
            if (bhPos != std::string::npos) {
                charClass.baseHealth = std::stof(block.substr(bhPos + 13));
            }
            
            size_t hplPos = block.find("healthPerLevel = ");
            if (hplPos != std::string::npos) {
                charClass.healthPerLevel = std::stof(block.substr(hplPos + 17));
            }
            
            size_t brPos = block.find("baseResource = ");
            if (brPos != std::string::npos) {
                charClass.baseResource = std::stof(block.substr(brPos + 15));
            }
            
            size_t rplPos = block.find("resourcePerLevel = ");
            if (rplPos != std::string::npos) {
                charClass.resourcePerLevel = std::stof(block.substr(rplPos + 19));
            }
            
            size_t sePos = block.find("secondaryEffectiveness = ");
            if (sePos != std::string::npos) {
                charClass.secondaryEffectiveness = std::stof(block.substr(sePos + 25));
            }
            
            // Extract allowed primary powersets
            size_t primaryPos = block.find("allowedPrimaryPowersets = {");
            if (primaryPos != std::string::npos) {
                size_t primaryEnd = block.find("}", primaryPos);
                std::string primaryBlock = block.substr(primaryPos + 27, primaryEnd - (primaryPos + 27));
                
                size_t psPos = 0;
                while ((psPos = primaryBlock.find("\"", psPos)) != std::string::npos) {
                    size_t psStart = psPos + 1;
                    size_t psEnd = primaryBlock.find("\"", psStart);
                    if (psEnd != std::string::npos) {
                        std::string psName = primaryBlock.substr(psStart, psEnd - psStart);
                        charClass.allowedPrimaryPowersetNames.push_back(psName);
                    }
                    psPos = psEnd + 1;
                }
            }
            
            // Extract allowed secondary powersets
            size_t secondaryPos = block.find("allowedSecondaryPowersets = {");
            if (secondaryPos != std::string::npos) {
                size_t secondaryEnd = block.find("}", secondaryPos);
                std::string secondaryBlock = block.substr(secondaryPos + 29, secondaryEnd - (secondaryPos + 29));
                
                size_t psPos = 0;
                while ((psPos = secondaryBlock.find("\"", psPos)) != std::string::npos) {
                    size_t psStart = psPos + 1;
                    size_t psEnd = secondaryBlock.find("\"", psStart);
                    if (psEnd != std::string::npos) {
                        std::string psName = secondaryBlock.substr(psStart, psEnd - psStart);
                        charClass.allowedSecondaryPowersetNames.push_back(psName);
                    }
                    psPos = psEnd + 1;
                }
            }
        }
        
        // Add class if valid
        if (!charClass.className.empty()) {
            m_classes.push_back(charClass);
            std::cout << "[ClassSystem]   Loaded class: " << charClass.className 
                      << " (" << archetypeToString(charClass.archetype) << ")" << std::endl;
        }
        
        pos = nameEnd + 1;
    }
    
    return !m_classes.empty();
}

void ClassSystem::resolveClassPowersets() {
    // For each class, resolve the powerset names to actual powerset objects
    for (auto& charClass : m_classes) {
        // Resolve primary powersets
        for (const auto& psName : charClass.allowedPrimaryPowersetNames) {
            auto it = m_powersetsByName.find(psName);
            if (it != m_powersetsByName.end()) {
                charClass.availablePrimaryPowersets.push_back(it->second);
            } else {
                std::cout << "[ClassSystem] Warning: Powerset not found: " << psName 
                          << " for class " << charClass.className << std::endl;
            }
        }
        
        // Resolve secondary powersets
        for (const auto& psName : charClass.allowedSecondaryPowersetNames) {
            auto it = m_powersetsByName.find(psName);
            if (it != m_powersetsByName.end()) {
                charClass.availableSecondaryPowersets.push_back(it->second);
            } else {
                std::cout << "[ClassSystem] Warning: Powerset not found: " << psName 
                          << " for class " << charClass.className << std::endl;
            }
        }
    }
}

// =============================================================================
// String Conversion Helpers
// =============================================================================

PowersetCategory ClassSystem::stringToCategory(const std::string& cat) const {
    if (cat == "ATTACK") return PowersetCategory::ATTACK;
    if (cat == "DEFENSE") return PowersetCategory::DEFENSE;
    if (cat == "HEALING") return PowersetCategory::HEALING;
    if (cat == "BUFF") return PowersetCategory::BUFF;
    if (cat == "SUMMON") return PowersetCategory::SUMMON;
    if (cat == "CONTROL") return PowersetCategory::CONTROL;
    return PowersetCategory::ATTACK;
}

ClassArchetype ClassSystem::stringToArchetype(const std::string& arch) const {
    if (arch == "ATTACKER") return ClassArchetype::ATTACKER;
    if (arch == "TANK") return ClassArchetype::TANK;
    if (arch == "HEALER") return ClassArchetype::HEALER;
    if (arch == "MASTERMIND") return ClassArchetype::MASTERMIND;
    return ClassArchetype::ATTACKER;
}

ElementalDamageType ClassSystem::stringToDamageType(const std::string& type) const {
    if (type == "FIRE") return ElementalDamageType::FIRE;
    if (type == "ICE") return ElementalDamageType::ICE;
    if (type == "LIGHTNING") return ElementalDamageType::LIGHTNING;
    if (type == "ARCANE") return ElementalDamageType::ARCANE;
    if (type == "SHADOW") return ElementalDamageType::SHADOW;
    if (type == "NECROTIC") return ElementalDamageType::NECROTIC;
    if (type == "HOLY") return ElementalDamageType::HOLY;
    if (type == "RADIANT") return ElementalDamageType::RADIANT;
    if (type == "ACID") return ElementalDamageType::ACID;
    if (type == "WATER") return ElementalDamageType::WATER;
    if (type == "NATURE") return ElementalDamageType::NATURE;
    if (type == "PSIONIC") return ElementalDamageType::PSIONIC;
    if (type == "CONJURING") return ElementalDamageType::CONJURING;
    if (type == "PIERCING") return ElementalDamageType::PIERCING;
    if (type == "SLASHING") return ElementalDamageType::SLASHING;
    if (type == "BLUNT") return ElementalDamageType::BLUNT;
    return ElementalDamageType::NONE;
}

// =============================================================================
// Lookup Methods
// =============================================================================
const CharacterClass* ClassSystem::getClass(const std::string& className) const {
    for (const auto& c : m_classes) {
        if (c.className == className) {
            return &c;
        }
    }
    return nullptr;
}

std::vector<Powerset> ClassSystem::getAvailablePrimaryPowersets(const std::string& className) const {
    const CharacterClass* cls = getClass(className);
    if (cls) {
        return cls->availablePrimaryPowersets;
    }
    return {};
}

std::vector<Powerset> ClassSystem::getAvailableSecondaryPowersets(const std::string& className) const {
    const CharacterClass* cls = getClass(className);
    if (cls) {
        return cls->availableSecondaryPowersets;
    }
    return {};
}

const Powerset* ClassSystem::findPowerset(const std::string& name) const {
    auto it = m_powersetsByName.find(name);
    if (it != m_powersetsByName.end()) {
        return &it->second;
    }
    return nullptr;
}

bool ClassSystem::isValidPowersetSelection(const std::string& className,
                                           const std::string& primaryPowerset,
                                           const std::string& secondaryPowerset) const {
    const CharacterClass* cls = getClass(className);
    if (!cls) return false;
    
    bool primaryValid = false;
    bool secondaryValid = false;
    
    for (const auto& ps : cls->availablePrimaryPowersets) {
        if (ps.name == primaryPowerset) {
            primaryValid = true;
            break;
        }
    }
    
    for (const auto& ps : cls->availableSecondaryPowersets) {
        if (ps.name == secondaryPowerset) {
            secondaryValid = true;
            break;
        }
    }
    
    return primaryValid && secondaryValid;
}

PowersetSelection ClassSystem::createPowersetSelection(const std::string& className,
                                                       const std::string& primaryPowerset,
                                                       const std::string& secondaryPowerset) const {
    PowersetSelection selection;
    
    const CharacterClass* cls = getClass(className);
    if (!cls) return selection;
    
    // Find primary
    for (const auto& ps : cls->availablePrimaryPowersets) {
        if (ps.name == primaryPowerset) {
            selection.primaryPowerset = ps;
            selection.primaryPowerset.isPrimary = true;
            selection.primaryPowerset.effectivenessModifier = 1.0f;
            break;
        }
    }
    
    // Find secondary
    for (const auto& ps : cls->availableSecondaryPowersets) {
        if (ps.name == secondaryPowerset) {
            selection.secondaryPowerset = ps;
            selection.secondaryPowerset.isPrimary = false;
            selection.secondaryPowerset.effectivenessModifier = cls->secondaryEffectiveness;
            break;
        }
    }
    
    selection.secondaryEffectiveness = cls->secondaryEffectiveness;
    
    return selection;
}

float ClassSystem::getSecondaryEffectiveness(ClassArchetype archetype) const {
    // Secondary effectiveness varies by archetype
    switch (archetype) {
        case ClassArchetype::ATTACKER:
            return 0.80f;  // Attackers get 80% on secondary
        case ClassArchetype::TANK:
            return 0.70f;  // Tanks get 70% on attack secondary
        case ClassArchetype::HEALER:
            return 0.65f;  // Healers get 65% on attack secondary
        case ClassArchetype::MASTERMIND:
            return 0.75f;  // Masterminds get 75% on buff/heal secondary
    }
    return 0.75f;
}

std::vector<Powerset> ClassSystem::getPowersetsByCategory(PowersetCategory category) const {
    std::vector<Powerset> result;
    for (const auto& ps : m_allPowersets) {
        if (ps.category == category) {
            result.push_back(ps);
        }
    }
    return result;
}

} // namespace character
} // namespace clonemine
