#include "LichAI.h"
#include "../../world/World.h"
#include "../../core/Random.h"
#include <algorithm>

namespace clonemine {

LichAI::LichAI(Entity* entity)
    : BossAI(entity)
    , m_currentPhase(1)
    , m_phaseChangeTriggered(false)
{
    m_maxHealth = entity->getMaxHealth();
}

void LichAI::update(float deltaTime, World& world) {
    // Check for phase transitions
    checkPhaseTransition(world);
    
    // Update based on current phase
    switch (m_currentPhase) {
        case 1:
            updatePhase1(deltaTime, world);
            break;
        case 2:
            updatePhase2(deltaTime, world);
            break;
        case 3:
            updatePhase3(deltaTime, world);
            break;
        case 4:
            updatePhase4(deltaTime, world);
            break;
    }
    
    // Update ability cooldowns
    for (auto& [name, cooldown] : m_abilityCooldowns) {
        cooldown -= deltaTime;
    }
}

void LichAI::checkPhaseTransition(World& world) {
    float healthPercent = m_entity->getHealth() / m_maxHealth;
    
    // Phase 2 at 75% HP
    if (m_currentPhase == 1 && healthPercent <= 0.75f) {
        enterPhase2(world);
    }
    // Phase 3 at 50% HP
    else if (m_currentPhase == 2 && healthPercent <= 0.50f) {
        enterPhase3(world);
    }
    // Phase 4 at 25% HP
    else if (m_currentPhase == 3 && healthPercent <= 0.25f) {
        enterPhase4(world);
    }
}

void LichAI::updatePhase1(float deltaTime, World& world) {
    auto target = findNearestPlayer(world);
    if (!target) return;
    
    // Cast Shadow Bolt
    if (canUseAbility("ShadowBolt")) {
        castShadowBolt(target, world);
        setAbilityCooldown("ShadowBolt", 2.0f);
    }
    
    // Teleport away if player gets close
    float distance = glm::distance(m_entity->getPosition(), target->getPosition());
    if (distance < 5.0f && canUseAbility("Teleport")) {
        teleportAway(target, world);
        setAbilityCooldown("Teleport", 8.0f);
    }
    
    // Curse of Weakness
    if (canUseAbility("CurseWeakness")) {
        castCurseWeakness(target, world);
        setAbilityCooldown("CurseWeakness", 15.0f);
    }
}

void LichAI::updatePhase2(float deltaTime, World& world) {
    auto target = findNearestPlayer(world);
    if (!target) return;
    
    // Continue Phase 1 abilities
    updatePhase1(deltaTime, world);
    
    // New Phase 2 ability: Ice Nova
    if (canUseAbility("IceNova")) {
        castIceNova(world);
        setAbilityCooldown("IceNova", 12.0f);
    }
    
    // Death Coil
    if (canUseAbility("DeathCoil")) {
        castDeathCoil(target, world);
        setAbilityCooldown("DeathCoil", 10.0f);
    }
}

void LichAI::updatePhase3(float deltaTime, World& world) {
    auto target = findNearestPlayer(world);
    if (!target) return;
    
    // Continue previous abilities
    updatePhase2(deltaTime, world);
    
    // Shadow Form (temporary immunity)
    if (canUseAbility("ShadowForm")) {
        enterShadowForm(world);
        setAbilityCooldown("ShadowForm", 30.0f);
    }
    
    // Life Drain
    if (canUseAbility("LifeDrain")) {
        castLifeDrain(target, world);
        setAbilityCooldown("LifeDrain", 8.0f);
    }
}

void LichAI::updatePhase4(float deltaTime, World& world) {
    auto target = findNearestPlayer(world);
    if (!target) return;
    
    // Continue all previous abilities with reduced cooldowns
    for (auto& [name, cooldown] : m_abilityCooldowns) {
        cooldown *= 0.5f; // Abilities ready faster in enrage
    }
    
    updatePhase3(deltaTime, world);
    
    // Apocalypse (ultimate ability)
    if (canUseAbility("Apocalypse")) {
        castApocalypse(world);
        setAbilityCooldown("Apocalypse", 45.0f);
    }
    
    // Rapid teleportation
    if (canUseAbility("RapidTeleport")) {
        teleportRandom(world);
        setAbilityCooldown("RapidTeleport", 3.0f);
    }
}

void LichAI::enterPhase2(World& world) {
    m_currentPhase = 2;
    m_entity->playAnimation("PhaseTransition");
    
    // Summon 6 Skeleton Archers
    for (int i = 0; i < 6; i++) {
        summonMinion("SkeletonArcher", world);
    }
    
    world.displayBossMessage("The Lich King calls upon his archers!");
}

void LichAI::enterPhase3(World& world) {
    m_currentPhase = 3;
    m_entity->playAnimation("PhaseTransition");
    
    // Raise 10 zombies
    for (int i = 0; i < 10; i++) {
        summonMinion("Zombie", world);
    }
    
    // Summon 2 Bone Golems
    for (int i = 0; i < 2; i++) {
        summonMinion("BoneGolem", world);
    }
    
    world.displayBossMessage("Rise, my servants! Let none survive!");
}

void LichAI::enterPhase4(World& world) {
    m_currentPhase = 4;
    m_entity->playAnimation("Enrage");
    
    // Respawn all previous minions
    for (int i = 0; i < 4; i++) {
        summonMinion("SkeletonWarrior", world);
    }
    for (int i = 0; i < 6; i++) {
        summonMinion("SkeletonArcher", world);
    }
    for (int i = 0; i < 5; i++) {
        summonMinion("Zombie", world);
    }
    
    world.displayBossMessage("You cannot defeat death itself!");
}

void LichAI::castShadowBolt(Entity* target, World& world) {
    m_entity->playAnimation("Cast");
    
    // Create projectile
    world.spawnProjectile(
        m_entity->getPosition() + glm::vec3(0, 2, 0),
        target->getPosition() + glm::vec3(0, 1, 0),
        50.0f,  // damage
        "Shadow",
        "ShadowBolt"  // visual effect
    );
}

void LichAI::castIceNova(World& world) {
    m_entity->playAnimation("CastAOE");
    
    // Damage and freeze all players in 10 block radius
    auto nearbyPlayers = world.getPlayersInRadius(m_entity->getPosition(), 10.0f);
    for (auto player : nearbyPlayers) {
        player->takeDamage(75.0f, "Ice");
        player->applyEffect("Frozen", 3.0f);
    }
    
    world.spawnEffect("IceNova", m_entity->getPosition());
}

void LichAI::castDeathCoil(Entity* target, World& world) {
    m_entity->playAnimation("Cast");
    
    // Damage target and heal self
    float damage = 60.0f;
    target->takeDamage(damage, "Shadow");
    m_entity->heal(damage * 0.5f);
    
    world.spawnEffect("DeathCoil", target->getPosition());
}

void LichAI::enterShadowForm(World& world) {
    m_entity->applyBuff("PhysicalImmunity", 10.0f);
    m_entity->setAlpha(0.3f);  // Become translucent
    world.spawnEffect("ShadowForm", m_entity->getPosition());
}

void LichAI::castLifeDrain(Entity* target, World& world) {
    m_entity->playAnimation("Channel");
    
    // Create beam effect
    world.spawnBeam(m_entity->getPosition(), target->getPosition(), "LifeDrain");
    
    // Drain over time (would need channeling system)
    float drainPerSecond = 30.0f;
    target->takeDamage(drainPerSecond, "Shadow");
    m_entity->heal(drainPerSecond * 0.75f);
}

void LichAI::castApocalypse(World& world) {
    m_entity->playAnimation("Ultimate");
    
    // Massive AOE that covers entire arena
    auto allPlayers = world.getPlayersInRadius(m_entity->getPosition(), 50.0f);
    for (auto player : allPlayers) {
        // Players must hide behind pillars to avoid damage
        if (!world.hasLineOfSight(m_entity->getPosition(), player->getPosition())) {
            // Player is safe behind cover
            continue;
        }
        player->takeDamage(200.0f, "Shadow");
    }
    
    world.spawnEffect("Apocalypse", m_entity->getPosition());
    world.displayBossMessage("APOCALYPSE!");
}

void LichAI::teleportAway(Entity* target, World& world) {
    // Teleport to opposite side of arena from player
    glm::vec3 direction = glm::normalize(m_entity->getPosition() - target->getPosition());
    glm::vec3 newPosition = m_entity->getPosition() + direction * 15.0f;
    
    world.spawnEffect("Teleport", m_entity->getPosition());
    m_entity->setPosition(newPosition);
    world.spawnEffect("Teleport", newPosition);
}

void LichAI::teleportRandom(World& world) {
    // Random teleport within arena
    float angle = Random::range(0.0f, 2.0f * 3.14159f);
    float radius = Random::range(5.0f, 15.0f);
    
    glm::vec3 offset(std::cos(angle) * radius, 0, std::sin(angle) * radius);
    glm::vec3 newPosition = m_entity->getPosition() + offset;
    
    world.spawnEffect("Teleport", m_entity->getPosition());
    m_entity->setPosition(newPosition);
    world.spawnEffect("Teleport", newPosition);
}

void LichAI::summonMinion(const std::string& minionType, World& world) {
    // Spawn minion near lich
    float angle = Random::range(0.0f, 2.0f * 3.14159f);
    float radius = 5.0f;
    
    glm::vec3 spawnPos = m_entity->getPosition() + glm::vec3(
        std::cos(angle) * radius,
        0,
        std::sin(angle) * radius
    );
    
    world.spawnEffect("Summon", spawnPos);
    auto minion = world.spawnMonster(minionType, spawnPos);
    m_summonedMinions.push_back(minion);
}

bool LichAI::canUseAbility(const std::string& abilityName) const {
    auto it = m_abilityCooldowns.find(abilityName);
    if (it == m_abilityCooldowns.end()) {
        return true;  // No cooldown set yet
    }
    return it->second <= 0.0f;
}

void LichAI::setAbilityCooldown(const std::string& abilityName, float cooldown) {
    m_abilityCooldowns[abilityName] = cooldown;
}

Entity* LichAI::findNearestPlayer(World& world) {
    auto players = world.getPlayersInRadius(m_entity->getPosition(), 100.0f);
    if (players.empty()) return nullptr;
    
    Entity* nearest = players[0];
    float nearestDist = glm::distance(m_entity->getPosition(), nearest->getPosition());
    
    for (auto player : players) {
        float dist = glm::distance(m_entity->getPosition(), player->getPosition());
        if (dist < nearestDist) {
            nearest = player;
            nearestDist = dist;
        }
    }
    
    return nearest;
}

void LichAI::onPhaseChange() {
    m_phaseChangeTriggered = true;
}

} // namespace clonemine
