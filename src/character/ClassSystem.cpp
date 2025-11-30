#include "ClassSystem.h"
#include <algorithm>
#include <iostream>

namespace clonemine {
namespace character {

ClassSystem::ClassSystem() {
}

void ClassSystem::initialize() {
    std::cout << "[ClassSystem] Initializing class system with dual powersets..." << std::endl;
    
    // Create all powersets first
    createAttackPowersets();
    createDefensePowersets();
    createHealingPowersets();
    createBuffPowersets();
    createSummonPowersets();
    createControlPowersets();
    
    // Then create classes with powerset assignments
    createAttackerClasses();
    createTankClasses();
    createHealerClasses();
    createMastermindClasses();
    
    std::cout << "[ClassSystem] Initialized " << m_classes.size() << " classes with " 
              << m_allPowersets.size() << " powersets" << std::endl;
}

// =============================================================================
// Attack Powersets - Damage dealing abilities
// =============================================================================
void ClassSystem::createAttackPowersets() {
    // Fire Blast - Pure fire damage
    Powerset fireBlast("Fire Blast", "Harness the power of flames to burn enemies", 
                       PowersetCategory::ATTACK, ElementalDamageType::FIRE);
    m_allPowersets.push_back(fireBlast);
    
    // Ice Assault - Ice damage with some control
    Powerset iceAssault("Ice Assault", "Freeze enemies with arctic cold", 
                        PowersetCategory::ATTACK, ElementalDamageType::ICE);
    m_allPowersets.push_back(iceAssault);
    
    // Lightning Strike - Fast lightning attacks
    Powerset lightningStrike("Lightning Strike", "Strike with the speed and power of lightning", 
                             PowersetCategory::ATTACK, ElementalDamageType::LIGHTNING);
    m_allPowersets.push_back(lightningStrike);
    
    // Arcane Power - Pure arcane damage
    Powerset arcanePower("Arcane Power", "Channel raw arcane energy", 
                         PowersetCategory::ATTACK, ElementalDamageType::ARCANE);
    m_allPowersets.push_back(arcanePower);
    
    // Shadow Strike - Dark magic attacks
    Powerset shadowStrike("Shadow Strike", "Command the darkness to strike enemies", 
                          PowersetCategory::ATTACK, ElementalDamageType::SHADOW);
    m_allPowersets.push_back(shadowStrike);
    
    // Necrotic Touch - Life drain and undead magic
    Powerset necroticTouch("Necrotic Touch", "Drain life force from enemies", 
                           PowersetCategory::ATTACK, ElementalDamageType::NECROTIC);
    m_allPowersets.push_back(necroticTouch);
    
    // Holy Smite - Holy damage attacks
    Powerset holySmite("Holy Smite", "Smite enemies with divine power", 
                       PowersetCategory::ATTACK, ElementalDamageType::HOLY);
    m_allPowersets.push_back(holySmite);
    
    // Acid Spray - Corrosive acid attacks
    Powerset acidSpray("Acid Spray", "Dissolve enemies with corrosive acid", 
                       PowersetCategory::ATTACK, ElementalDamageType::ACID);
    m_allPowersets.push_back(acidSpray);
    
    // Water Torrent - Water-based attacks
    Powerset waterTorrent("Water Torrent", "Unleash the fury of water", 
                          PowersetCategory::ATTACK, ElementalDamageType::WATER);
    m_allPowersets.push_back(waterTorrent);
    
    // Psionic Assault - Mental attacks
    Powerset psionicAssault("Psionic Assault", "Attack the mind directly", 
                            PowersetCategory::ATTACK, ElementalDamageType::PSIONIC);
    m_allPowersets.push_back(psionicAssault);
    
    // Nature's Wrath - Nature damage
    Powerset naturesWrath("Nature's Wrath", "Command nature to attack enemies", 
                          PowersetCategory::ATTACK, ElementalDamageType::NATURE);
    m_allPowersets.push_back(naturesWrath);
}

// =============================================================================
// Defense Powersets - Protection and mitigation
// =============================================================================
void ClassSystem::createDefensePowersets() {
    // Ice Armor - Ice-based defenses with barriers
    Powerset iceArmor("Ice Armor", "Protect yourself with ice shields and barriers", 
                      PowersetCategory::DEFENSE, ElementalDamageType::ICE);
    m_allPowersets.push_back(iceArmor);
    
    // Radiant Shield - Light-based protection
    Powerset radiantShield("Radiant Shield", "Create protective barriers of light", 
                           PowersetCategory::DEFENSE, ElementalDamageType::RADIANT);
    m_allPowersets.push_back(radiantShield);
    
    // Stone Skin - Earth/Nature based armor
    Powerset stoneSkin("Stone Skin", "Harden your skin like stone", 
                       PowersetCategory::DEFENSE, ElementalDamageType::NATURE);
    m_allPowersets.push_back(stoneSkin);
    
    // Willpower - Mental fortitude
    Powerset willpower("Willpower", "Resist damage through sheer mental strength", 
                       PowersetCategory::DEFENSE, ElementalDamageType::PSIONIC);
    m_allPowersets.push_back(willpower);
    
    // Dark Regeneration - Shadow-based damage absorption
    Powerset darkRegeneration("Dark Regeneration", "Absorb darkness to resist damage", 
                              PowersetCategory::DEFENSE, ElementalDamageType::SHADOW);
    m_allPowersets.push_back(darkRegeneration);
}

// =============================================================================
// Healing Powersets - Health restoration
// =============================================================================
void ClassSystem::createHealingPowersets() {
    // Holy Blessing - Divine healing
    Powerset holyBlessing("Holy Blessing", "Heal wounds with divine power", 
                          PowersetCategory::HEALING, ElementalDamageType::HOLY);
    m_allPowersets.push_back(holyBlessing);
    
    // Radiant Restoration - Light-based healing
    Powerset radiantRestoration("Radiant Restoration", "Restore health with pure light", 
                                PowersetCategory::HEALING, ElementalDamageType::RADIANT);
    m_allPowersets.push_back(radiantRestoration);
    
    // Nature's Embrace - Nature healing
    Powerset naturesEmbrace("Nature's Embrace", "Heal with the power of nature", 
                            PowersetCategory::HEALING, ElementalDamageType::NATURE);
    m_allPowersets.push_back(naturesEmbrace);
    
    // Water Healing - Water-based restoration
    Powerset waterHealing("Water Healing", "Cleanse and heal with pure water", 
                          PowersetCategory::HEALING, ElementalDamageType::WATER);
    m_allPowersets.push_back(waterHealing);
}

// =============================================================================
// Buff Powersets - Stat enhancement and team support
// =============================================================================
void ClassSystem::createBuffPowersets() {
    // Arcane Empowerment - Arcane stat boosts
    Powerset arcaneEmpowerment("Arcane Empowerment", "Enhance allies with arcane power", 
                               PowersetCategory::BUFF, ElementalDamageType::ARCANE);
    m_allPowersets.push_back(arcaneEmpowerment);
    
    // Holy Aura - Divine team buffs
    Powerset holyAura("Holy Aura", "Bless allies with divine enhancement", 
                      PowersetCategory::BUFF, ElementalDamageType::HOLY);
    m_allPowersets.push_back(holyAura);
    
    // Nature's Gift - Nature stat boosts
    Powerset naturesGift("Nature's Gift", "Grant allies the strength of nature", 
                         PowersetCategory::BUFF, ElementalDamageType::NATURE);
    m_allPowersets.push_back(naturesGift);
    
    // Psionic Boost - Mental enhancement
    Powerset psionicBoost("Psionic Boost", "Enhance minds and focus", 
                          PowersetCategory::BUFF, ElementalDamageType::PSIONIC);
    m_allPowersets.push_back(psionicBoost);
}

// =============================================================================
// Summon Powersets - Pet and minion summoning
// =============================================================================
void ClassSystem::createSummonPowersets() {
    // Necromancy - Summon undead minions
    Powerset necromancy("Necromancy", "Raise undead minions to fight for you", 
                        PowersetCategory::SUMMON, ElementalDamageType::NECROTIC);
    m_allPowersets.push_back(necromancy);
    
    // Conjuration - Summon arcane creatures
    Powerset conjuration("Conjuration", "Summon magical constructs and creatures", 
                         PowersetCategory::SUMMON, ElementalDamageType::CONJURING);
    m_allPowersets.push_back(conjuration);
    
    // Beast Mastery - Summon nature beasts
    Powerset beastMastery("Beast Mastery", "Call upon beasts of nature to aid you", 
                          PowersetCategory::SUMMON, ElementalDamageType::NATURE);
    m_allPowersets.push_back(beastMastery);
    
    // Demon Summoning - Summon shadow demons
    Powerset demonSummoning("Demon Summoning", "Summon demons from the shadow realm", 
                            PowersetCategory::SUMMON, ElementalDamageType::SHADOW);
    m_allPowersets.push_back(demonSummoning);
}

// =============================================================================
// Control Powersets - Crowd control and debuffs
// =============================================================================
void ClassSystem::createControlPowersets() {
    // Mind Control - Psionic crowd control
    Powerset mindControl("Mind Control", "Dominate and control enemy minds", 
                         PowersetCategory::CONTROL, ElementalDamageType::PSIONIC);
    m_allPowersets.push_back(mindControl);
    
    // Ice Control - Freeze and slow enemies
    Powerset iceControl("Ice Control", "Freeze enemies in place", 
                        PowersetCategory::CONTROL, ElementalDamageType::ICE);
    m_allPowersets.push_back(iceControl);
    
    // Shadow Binding - Dark crowd control
    Powerset shadowBinding("Shadow Binding", "Bind enemies with dark tendrils", 
                           PowersetCategory::CONTROL, ElementalDamageType::SHADOW);
    m_allPowersets.push_back(shadowBinding);
}

// =============================================================================
// Attacker Classes - Primary: Attack, Secondary: Attack or Heal
// =============================================================================
void ClassSystem::createAttackerClasses() {
    // Mage - Fire/Ice/Lightning attacks, can take healing secondary
    CharacterClass mage;
    mage.className = "Mage";
    mage.description = "Master of elemental magic";
    mage.archetype = ClassArchetype::ATTACKER;
    mage.primaryStat = "intelligence";
    mage.resourceType = "mana";
    mage.baseHealth = 80.0f;
    mage.healthPerLevel = 8.0f;
    mage.baseResource = 150.0f;
    mage.resourcePerLevel = 10.0f;
    
    // Mage primaries - Attack sets
    for (const auto& ps : m_allPowersets) {
        if (ps.category == PowersetCategory::ATTACK) {
            mage.availablePrimaryPowersets.push_back(ps);
        }
    }
    // Mage secondaries - Attack or Healing (mages can heal)
    for (const auto& ps : m_allPowersets) {
        if (ps.category == PowersetCategory::ATTACK || 
            ps.category == PowersetCategory::HEALING) {
            mage.availableSecondaryPowersets.push_back(ps);
        }
    }
    m_classes.push_back(mage);
    
    // Warlock - Shadow/Necrotic focus
    CharacterClass warlock;
    warlock.className = "Warlock";
    warlock.description = "Dark magic wielder";
    warlock.archetype = ClassArchetype::ATTACKER;
    warlock.primaryStat = "intelligence";
    warlock.resourceType = "mana";
    warlock.baseHealth = 85.0f;
    warlock.healthPerLevel = 8.0f;
    warlock.baseResource = 140.0f;
    warlock.resourcePerLevel = 9.0f;
    
    // Warlock primaries - Dark attack sets
    for (const auto& ps : m_allPowersets) {
        if (ps.category == PowersetCategory::ATTACK &&
            (ps.damageType == ElementalDamageType::SHADOW ||
             ps.damageType == ElementalDamageType::NECROTIC ||
             ps.damageType == ElementalDamageType::FIRE)) {
            warlock.availablePrimaryPowersets.push_back(ps);
        }
    }
    // Warlock secondaries - Attack or Control
    for (const auto& ps : m_allPowersets) {
        if (ps.category == PowersetCategory::ATTACK ||
            ps.category == PowersetCategory::CONTROL) {
            warlock.availableSecondaryPowersets.push_back(ps);
        }
    }
    m_classes.push_back(warlock);
    
    // Psion - Mental attacker
    CharacterClass psion;
    psion.className = "Psion";
    psion.description = "Master of mental powers";
    psion.archetype = ClassArchetype::ATTACKER;
    psion.primaryStat = "wisdom";
    psion.resourceType = "focus";
    psion.baseHealth = 75.0f;
    psion.healthPerLevel = 7.0f;
    psion.baseResource = 120.0f;
    psion.resourcePerLevel = 8.0f;
    
    // Psion primaries - Psionic attacks
    for (const auto& ps : m_allPowersets) {
        if (ps.category == PowersetCategory::ATTACK &&
            ps.damageType == ElementalDamageType::PSIONIC) {
            psion.availablePrimaryPowersets.push_back(ps);
        }
    }
    // Psion secondaries - Attack or Control
    for (const auto& ps : m_allPowersets) {
        if (ps.category == PowersetCategory::ATTACK ||
            ps.category == PowersetCategory::CONTROL) {
            psion.availableSecondaryPowersets.push_back(ps);
        }
    }
    m_classes.push_back(psion);
}

// =============================================================================
// Tank Classes - Primary: Defense, Secondary: Attack
// =============================================================================
void ClassSystem::createTankClasses() {
    // Guardian - Heavy armor tank
    CharacterClass guardian;
    guardian.className = "Guardian";
    guardian.description = "Stalwart defender with shields";
    guardian.archetype = ClassArchetype::TANK;
    guardian.primaryStat = "constitution";
    guardian.resourceType = "energy";
    guardian.baseHealth = 150.0f;
    guardian.healthPerLevel = 15.0f;
    guardian.baseResource = 100.0f;
    guardian.resourcePerLevel = 5.0f;
    
    // Guardian primaries - Defense sets
    for (const auto& ps : m_allPowersets) {
        if (ps.category == PowersetCategory::DEFENSE) {
            guardian.availablePrimaryPowersets.push_back(ps);
        }
    }
    // Guardian secondaries - Attack sets
    for (const auto& ps : m_allPowersets) {
        if (ps.category == PowersetCategory::ATTACK) {
            guardian.availableSecondaryPowersets.push_back(ps);
        }
    }
    m_classes.push_back(guardian);
    
    // Frost Knight - Ice-based tank
    CharacterClass frostKnight;
    frostKnight.className = "Frost Knight";
    frostKnight.description = "Armored warrior of ice";
    frostKnight.archetype = ClassArchetype::TANK;
    frostKnight.primaryStat = "constitution";
    frostKnight.resourceType = "energy";
    frostKnight.baseHealth = 140.0f;
    frostKnight.healthPerLevel = 14.0f;
    frostKnight.baseResource = 110.0f;
    frostKnight.resourcePerLevel = 6.0f;
    
    // Frost Knight primaries - Ice defense sets
    for (const auto& ps : m_allPowersets) {
        if (ps.category == PowersetCategory::DEFENSE &&
            ps.damageType == ElementalDamageType::ICE) {
            frostKnight.availablePrimaryPowersets.push_back(ps);
        }
    }
    // Frost Knight secondaries - Ice or physical attack
    for (const auto& ps : m_allPowersets) {
        if (ps.category == PowersetCategory::ATTACK &&
            ps.damageType == ElementalDamageType::ICE) {
            frostKnight.availableSecondaryPowersets.push_back(ps);
        }
    }
    m_classes.push_back(frostKnight);
}

// =============================================================================
// Healer Classes - Primary: Healing, Secondary: Attack (reduced)
// =============================================================================
void ClassSystem::createHealerClasses() {
    // Cleric - Divine healer
    CharacterClass cleric;
    cleric.className = "Cleric";
    cleric.description = "Divine healer and holy warrior";
    cleric.archetype = ClassArchetype::HEALER;
    cleric.primaryStat = "wisdom";
    cleric.resourceType = "mana";
    cleric.baseHealth = 100.0f;
    cleric.healthPerLevel = 10.0f;
    cleric.baseResource = 130.0f;
    cleric.resourcePerLevel = 8.0f;
    
    // Cleric primaries - Healing sets
    for (const auto& ps : m_allPowersets) {
        if (ps.category == PowersetCategory::HEALING) {
            cleric.availablePrimaryPowersets.push_back(ps);
        }
    }
    // Cleric secondaries - Holy attack (reduced effectiveness)
    for (const auto& ps : m_allPowersets) {
        if (ps.category == PowersetCategory::ATTACK &&
            (ps.damageType == ElementalDamageType::HOLY ||
             ps.damageType == ElementalDamageType::RADIANT)) {
            cleric.availableSecondaryPowersets.push_back(ps);
        }
    }
    m_classes.push_back(cleric);
    
    // Druid - Nature healer
    CharacterClass druid;
    druid.className = "Druid";
    druid.description = "Healer of nature";
    druid.archetype = ClassArchetype::HEALER;
    druid.primaryStat = "wisdom";
    druid.resourceType = "mana";
    druid.baseHealth = 95.0f;
    druid.healthPerLevel = 9.0f;
    druid.baseResource = 135.0f;
    druid.resourcePerLevel = 9.0f;
    
    // Druid primaries - Nature/Water healing
    for (const auto& ps : m_allPowersets) {
        if (ps.category == PowersetCategory::HEALING &&
            (ps.damageType == ElementalDamageType::NATURE ||
             ps.damageType == ElementalDamageType::WATER)) {
            druid.availablePrimaryPowersets.push_back(ps);
        }
    }
    // Druid secondaries - Nature attack
    for (const auto& ps : m_allPowersets) {
        if (ps.category == PowersetCategory::ATTACK &&
            ps.damageType == ElementalDamageType::NATURE) {
            druid.availableSecondaryPowersets.push_back(ps);
        }
    }
    m_classes.push_back(druid);
}

// =============================================================================
// Mastermind Classes - Primary: Summon, Secondary: Buff/Heal
// =============================================================================
void ClassSystem::createMastermindClasses() {
    // Necromancer - Summons undead
    CharacterClass necromancer;
    necromancer.className = "Necromancer";
    necromancer.description = "Master of undead minions";
    necromancer.archetype = ClassArchetype::MASTERMIND;
    necromancer.primaryStat = "intelligence";
    necromancer.resourceType = "mana";
    necromancer.baseHealth = 70.0f;
    necromancer.healthPerLevel = 7.0f;
    necromancer.baseResource = 140.0f;
    necromancer.resourcePerLevel = 9.0f;
    
    // Necromancer primaries - Necromancy summons
    for (const auto& ps : m_allPowersets) {
        if (ps.category == PowersetCategory::SUMMON &&
            ps.damageType == ElementalDamageType::NECROTIC) {
            necromancer.availablePrimaryPowersets.push_back(ps);
        }
    }
    // Necromancer secondaries - Buff or minor heal
    for (const auto& ps : m_allPowersets) {
        if (ps.category == PowersetCategory::BUFF ||
            ps.category == PowersetCategory::HEALING) {
            necromancer.availableSecondaryPowersets.push_back(ps);
        }
    }
    m_classes.push_back(necromancer);
    
    // Conjurer - Summons magical creatures
    CharacterClass conjurer;
    conjurer.className = "Conjurer";
    conjurer.description = "Summons arcane creatures";
    conjurer.archetype = ClassArchetype::MASTERMIND;
    conjurer.primaryStat = "intelligence";
    conjurer.resourceType = "mana";
    conjurer.baseHealth = 75.0f;
    conjurer.healthPerLevel = 7.0f;
    conjurer.baseResource = 145.0f;
    conjurer.resourcePerLevel = 10.0f;
    
    // Conjurer primaries - Conjuration summons
    for (const auto& ps : m_allPowersets) {
        if (ps.category == PowersetCategory::SUMMON &&
            ps.damageType == ElementalDamageType::CONJURING) {
            conjurer.availablePrimaryPowersets.push_back(ps);
        }
    }
    // Conjurer secondaries - Buff sets
    for (const auto& ps : m_allPowersets) {
        if (ps.category == PowersetCategory::BUFF) {
            conjurer.availableSecondaryPowersets.push_back(ps);
        }
    }
    m_classes.push_back(conjurer);
    
    // Beast Lord - Summons nature beasts
    CharacterClass beastLord;
    beastLord.className = "Beast Lord";
    beastLord.description = "Commands nature's creatures";
    beastLord.archetype = ClassArchetype::MASTERMIND;
    beastLord.primaryStat = "wisdom";
    beastLord.resourceType = "focus";
    beastLord.baseHealth = 85.0f;
    beastLord.healthPerLevel = 8.0f;
    beastLord.baseResource = 120.0f;
    beastLord.resourcePerLevel = 7.0f;
    
    // Beast Lord primaries - Beast summons
    for (const auto& ps : m_allPowersets) {
        if (ps.category == PowersetCategory::SUMMON &&
            ps.damageType == ElementalDamageType::NATURE) {
            beastLord.availablePrimaryPowersets.push_back(ps);
        }
    }
    // Beast Lord secondaries - Nature buff/heal
    for (const auto& ps : m_allPowersets) {
        if ((ps.category == PowersetCategory::BUFF || 
             ps.category == PowersetCategory::HEALING) &&
            ps.damageType == ElementalDamageType::NATURE) {
            beastLord.availableSecondaryPowersets.push_back(ps);
        }
    }
    m_classes.push_back(beastLord);
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
    for (const auto& ps : m_allPowersets) {
        if (ps.name == name) {
            return &ps;
        }
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
            selection.secondaryPowerset.effectivenessModifier = getSecondaryEffectiveness(cls->archetype);
            break;
        }
    }
    
    selection.secondaryEffectiveness = getSecondaryEffectiveness(cls->archetype);
    
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
