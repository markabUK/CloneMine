#pragma once

#include "../MonsterAI.h"
#include <vector>
#include <memory>

namespace clonemine {

class LichAI : public MonsterAI {
public:
    LichAI(const glm::vec3& spawnPos)
        : MonsterAI("Lich King", spawnPos) {
        m_maxHealth = 10000.0f;
        m_health = m_maxHealth;
        m_moveSpeed = 4.0f;
        m_aggroRange = 30.0f;
        m_attackRange = 20.0f;
        m_currentPhase = 1;
        
        // Setup abilities
        Ability shadowBolt;
        shadowBolt.name = "Shadow Bolt";
        shadowBolt.cooldown = 3.0f;
        shadowBolt.range = 20.0f;
        shadowBolt.damageInfo = DamageCalculation::createElementalDamage(150.0f, ElementalDamageType::SHADOW);
        m_abilities.push_back(shadowBolt);
        
        Ability iceNova;
        iceNova.name = "Ice Nova";
        iceNova.cooldown = 12.0f;
        iceNova.range = 10.0f;
        iceNova.damageInfo = DamageCalculation::createElementalDamage(300.0f, ElementalDamageType::ICE);
        m_abilities.push_back(iceNova);
        
        Ability deathCoil;
        deathCoil.name = "Death Coil";
        deathCoil.cooldown = 8.0f;
        deathCoil.range = 15.0f;
        deathCoil.damageInfo = DamageCalculation::createElementalDamage(200.0f, ElementalDamageType::SHADOW);
        m_abilities.push_back(deathCoil);
        
        // Lich has high shadow resistance, vulnerable to holy
        m_resistances.shadowResist = 0.75f;
        m_resistances.holyResist = -0.5f;
        m_resistances.fireResist = 0.3f;
        m_resistances.iceResist = 0.5f;
        m_resistances.piercingResist = 0.3f;
        m_resistances.slashingResist = 0.3f;
        m_resistances.bluntResist = 0.3f;
    }
    
    void update(float deltaTime, const glm::vec3& playerPos) override {
        // Check for phase transitions
        float healthPercent = m_health / m_maxHealth;
        
        if (healthPercent <= 0.75f && m_currentPhase == 1) {
            enterPhase2();
        } else if (healthPercent <= 0.5f && m_currentPhase == 2) {
            enterPhase3();
        } else if (healthPercent <= 0.25f && m_currentPhase == 3) {
            enterPhase4();
        }
        
        // Update phase-specific mechanics
        updatePhase(deltaTime, playerPos);
        
        MonsterAI::update(deltaTime, playerPos);
    }
    
protected:
    void updatePhase(float deltaTime, const glm::vec3& playerPos) {
        m_phaseTimer += deltaTime;
        
        switch (m_currentPhase) {
            case 1:
                // Teleport away if player gets too close
                if (glm::distance(m_position, playerPos) < 5.0f && m_phaseTimer > 5.0f) {
                    teleportAway(playerPos);
                    m_phaseTimer = 0.0f;
                }
                break;
                
            case 2:
                // Periodically go into shadow form (immune to physical)
                if (m_phaseTimer > 15.0f) {
                    m_inShadowForm = !m_inShadowForm;
                    if (m_inShadowForm) {
                        // Become immune to physical damage
                        m_resistances.piercingResist = 1.0f;
                        m_resistances.slashingResist = 1.0f;
                        m_resistances.bluntResist = 1.0f;
                    } else {
                        m_resistances.piercingResist = 0.3f;
                        m_resistances.slashingResist = 0.3f;
                        m_resistances.bluntResist = 0.3f;
                    }
                    m_phaseTimer = 0.0f;
                }
                break;
                
            case 3:
                // Summon Bone Golems periodically
                if (m_phaseTimer > 20.0f) {
                    summonBoneGolems();
                    m_phaseTimer = 0.0f;
                }
                break;
                
            case 4:
                // Apocalypse spell
                if (m_phaseTimer > 30.0f) {
                    castApocalypse();
                    m_phaseTimer = 0.0f;
                }
                break;
        }
    }
    
    void enterPhase2() {
        m_currentPhase = 2;
        m_phaseTimer = 0.0f;
        // Summon 6 Skeleton Archers
        summonMinions("Skeleton Archer", 6);
    }
    
    void enterPhase3() {
        m_currentPhase = 3;
        m_phaseTimer = 0.0f;
        // Raise 10 zombies
        summonMinions("Zombie", 10);
    }
    
    void enterPhase4() {
        m_currentPhase = 4;
        m_phaseTimer = 0.0f;
        // Respawn all previous minions
        summonMinions("Skeleton Warrior", 4);
        summonMinions("Skeleton Archer", 6);
        summonMinions("Zombie", 10);
    }
    
    void teleportAway(const glm::vec3& playerPos) {
        // Teleport to a random location away from player
        float angle = static_cast<float>(rand()) / RAND_MAX * 2.0f * 3.14159f;
        float dist = 15.0f + static_cast<float>(rand()) / RAND_MAX * 10.0f;
        m_position = playerPos + glm::vec3(cos(angle) * dist, 0, sin(angle) * dist);
    }
    
    void summonMinions(const std::string& type, int count) {
        // This would interface with the spawn system
        // For now, just track that we summoned
        m_activeMinions += count;
    }
    
    void summonBoneGolems() {
        summonMinions("Bone Golem", 2);
    }
    
    void castApocalypse() {
        // Massive AOE attack
        // Would trigger particle effects and area damage
    }
    
    bool canFlee() const override {
        return false;  // Bosses never flee
    }
    
    int m_currentPhase = 1;
    float m_phaseTimer = 0.0f;
    bool m_inShadowForm = false;
    int m_activeMinions = 0;
};

} // namespace clonemine
