#include "WorldRenderer.h"
#include <iostream>
#include <algorithm>
#include <cmath>

namespace clonemine {

// ============================================================================
// WorldRenderer Implementation
// ============================================================================

WorldRenderer::WorldRenderer() {
    // Initialize FPS history
    for (int i = 0; i < 60; ++i) {
        m_fpsHistory[i] = 60.0f;
    }
}

WorldRenderer::~WorldRenderer() {
    shutdown();
}

bool WorldRenderer::initialize() {
    if (m_initialized) {
        return true;
    }
    
    std::cout << "[WorldRenderer] Initializing with render settings:" << std::endl;
    std::cout << "[WorldRenderer]   World render distance: " << m_settings.worldRenderDistance << " units" << std::endl;
    std::cout << "[WorldRenderer]   Player render distance: " << m_settings.playerRenderDistance << " units" << std::endl;
    std::cout << "[WorldRenderer]   Monster render distance: " << m_settings.monsterRenderDistance << " units" << std::endl;
    std::cout << "[WorldRenderer]   Spell render distance: " << m_settings.spellRenderDistance << " units" << std::endl;
    std::cout << "[WorldRenderer]   Ability render distance: " << m_settings.abilityRenderDistance << " units" << std::endl;
    std::cout << "[WorldRenderer]   Auto-adjust quality: " << (m_settings.autoAdjustQuality ? "ON" : "OFF") << std::endl;
    std::cout << "[WorldRenderer]   Target FPS: " << m_settings.targetFPS << std::endl;
    
    m_initialized = true;
    return true;
}

void WorldRenderer::shutdown() {
    if (!m_initialized) {
        return;
    }
    
    clearAllEntities();
    m_initialized = false;
    std::cout << "[WorldRenderer] Shutdown complete" << std::endl;
}

void WorldRenderer::setRenderSettings(const RenderSettings& settings) {
    m_settings = settings;
    std::cout << "[WorldRenderer] Render settings updated:" << std::endl;
    std::cout << "[WorldRenderer]   World: " << m_settings.worldRenderDistance << " units" << std::endl;
    std::cout << "[WorldRenderer]   Players: " << m_settings.playerRenderDistance << " units" << std::endl;
    std::cout << "[WorldRenderer]   Monsters: " << m_settings.monsterRenderDistance << " units" << std::endl;
    std::cout << "[WorldRenderer]   Spells: " << m_settings.spellRenderDistance << " units" << std::endl;
    std::cout << "[WorldRenderer]   Detail level: " << m_settings.detailLevel << std::endl;
}

void WorldRenderer::adjustRenderDistanceForFPS(float currentFPS) {
    // Update FPS history
    m_fpsHistory[m_fpsHistoryIndex] = currentFPS;
    m_fpsHistoryIndex = (m_fpsHistoryIndex + 1) % 60;
    
    // Calculate average FPS
    float sum = 0.0f;
    for (int i = 0; i < 60; ++i) {
        sum += m_fpsHistory[i];
    }
    m_averageFPS = sum / 60.0f;
    
    if (!m_settings.autoAdjustQuality) {
        return;
    }
    
    // Adjust render distances based on FPS
    if (m_averageFPS < m_settings.minFPS) {
        // FPS too low - reduce render distances by 10%
        float reduction = 0.9f;
        m_settings.worldRenderDistance *= reduction;
        m_settings.playerRenderDistance *= reduction;
        m_settings.monsterRenderDistance *= reduction;
        m_settings.spellRenderDistance *= reduction;
        m_settings.abilityRenderDistance *= reduction;
        m_settings.npcRenderDistance *= reduction;
        m_settings.projectileRenderDistance *= reduction;
        m_settings.particleRenderDistance *= reduction;
        
        // Enforce minimums
        m_settings.worldRenderDistance = std::max(64.0f, m_settings.worldRenderDistance);
        m_settings.playerRenderDistance = std::max(30.0f, m_settings.playerRenderDistance);
        m_settings.monsterRenderDistance = std::max(25.0f, m_settings.monsterRenderDistance);
        m_settings.spellRenderDistance = std::max(30.0f, m_settings.spellRenderDistance);
        m_settings.abilityRenderDistance = std::max(25.0f, m_settings.abilityRenderDistance);
        
        std::cout << "[WorldRenderer] FPS (" << m_averageFPS << ") below minimum (" 
                  << m_settings.minFPS << "), reducing render distances" << std::endl;
    }
    else if (m_averageFPS > m_settings.targetFPS * 1.2f) {
        // FPS well above target - can increase render distances by 5%
        float increase = 1.05f;
        m_settings.worldRenderDistance = std::min(512.0f, m_settings.worldRenderDistance * increase);
        m_settings.playerRenderDistance = std::min(200.0f, m_settings.playerRenderDistance * increase);
        m_settings.monsterRenderDistance = std::min(150.0f, m_settings.monsterRenderDistance * increase);
        m_settings.spellRenderDistance = std::min(200.0f, m_settings.spellRenderDistance * increase);
        m_settings.abilityRenderDistance = std::min(150.0f, m_settings.abilityRenderDistance * increase);
    }
}

void WorldRenderer::setCameraPosition(const glm::vec3& position) {
    m_cameraPosition = position;
}

void WorldRenderer::setCameraDirection(const glm::vec3& direction) {
    m_cameraDirection = glm::normalize(direction);
}

void WorldRenderer::beginFrame() {
    m_renderedEntityCount = 0;
    m_culledEntityCount = 0;
}

void WorldRenderer::endFrame() {
    // Clear render queues for next frame
    m_playerQueue.clear();
    m_monsterQueue.clear();
    m_npcQueue.clear();
    m_projectileQueue.clear();
    m_spellEffectQueue.clear();
    m_abilityEffectQueue.clear();
}

void WorldRenderer::renderWorld(const World& world) {
    // Render world terrain within render distance
    std::cout << "[WorldRenderer] Rendering world terrain (distance: " 
              << m_settings.worldRenderDistance << " units)" << std::endl;
    // Actual rendering would be done by the Vulkan renderer
}

void WorldRenderer::renderPlayers(const std::vector<RenderEntity>& players) {
    for (const auto& player : players) {
        if (!shouldCull(player.position, RenderEntityType::PLAYER)) {
            renderPlayer(player);
            m_renderedEntityCount++;
        } else {
            m_culledEntityCount++;
        }
    }
}

void WorldRenderer::renderPlayer(const RenderEntity& player) {
    if (!player.visible) return;
    
    float distance = getDistanceFromCamera(player.position);
    std::cout << "[WorldRenderer] Rendering player '" << player.modelName 
              << "' at distance " << distance << " units" << std::endl;
    // Actual rendering would use Vulkan/graphics API
}

void WorldRenderer::renderMonsters(const std::vector<RenderEntity>& monsters) {
    for (const auto& monster : monsters) {
        if (!shouldCull(monster.position, RenderEntityType::MONSTER)) {
            renderMonster(monster);
            m_renderedEntityCount++;
        } else {
            m_culledEntityCount++;
        }
    }
}

void WorldRenderer::renderMonster(const RenderEntity& monster) {
    if (!monster.visible) return;
    
    float distance = getDistanceFromCamera(monster.position);
    std::cout << "[WorldRenderer] Rendering monster '" << monster.modelName 
              << "' at distance " << distance << " units" << std::endl;
}

void WorldRenderer::renderNPCs(const std::vector<RenderEntity>& npcs) {
    for (const auto& npc : npcs) {
        if (!shouldCull(npc.position, RenderEntityType::NPC)) {
            renderNPC(npc);
            m_renderedEntityCount++;
        } else {
            m_culledEntityCount++;
        }
    }
}

void WorldRenderer::renderNPC(const RenderEntity& npc) {
    if (!npc.visible) return;
    
    float distance = getDistanceFromCamera(npc.position);
    std::cout << "[WorldRenderer] Rendering NPC '" << npc.modelName 
              << "' at distance " << distance << " units" << std::endl;
}

void WorldRenderer::renderProjectiles(const std::vector<RenderEntity>& projectiles) {
    for (const auto& projectile : projectiles) {
        if (!shouldCull(projectile.position, RenderEntityType::PROJECTILE)) {
            renderProjectile(projectile);
            m_renderedEntityCount++;
        } else {
            m_culledEntityCount++;
        }
    }
}

void WorldRenderer::renderProjectile(const RenderEntity& projectile) {
    if (!projectile.visible) return;
    
    float distance = getDistanceFromCamera(projectile.position);
    std::cout << "[WorldRenderer] Rendering projectile '" << projectile.modelName 
              << "' (" << projectile.effectType << ") at distance " << distance << " units" << std::endl;
}

void WorldRenderer::renderSpellEffects(const std::vector<SpellEffect>& effects) {
    if (!m_settings.spellEffectsEnabled) return;
    
    for (const auto& effect : effects) {
        if (!shouldCull(effect.position, RenderEntityType::SPELL_EFFECT)) {
            renderSpellEffect(effect);
            m_renderedEntityCount++;
        } else {
            m_culledEntityCount++;
        }
    }
}

void WorldRenderer::renderSpellEffect(const SpellEffect& effect) {
    float distance = getDistanceFromCamera(effect.position);
    std::cout << "[WorldRenderer] Rendering spell effect '" << effect.spellName 
              << "' (" << effect.schoolName << ") at distance " << distance 
              << " units, time remaining: " << effect.timeRemaining << "s" << std::endl;
    
    // Render projectile if it's a projectile spell
    if (effect.isProjectile) {
        std::cout << "[WorldRenderer]   -> Projectile heading to target" << std::endl;
    }
    
    // Render area effect
    if (effect.isAreaEffect) {
        std::cout << "[WorldRenderer]   -> Area effect radius: " << effect.radius << std::endl;
    }
}

void WorldRenderer::renderAbilityEffects(const std::vector<AbilityEffect>& effects) {
    if (!m_settings.abilityEffectsEnabled) return;
    
    for (const auto& effect : effects) {
        if (!shouldCull(effect.position, RenderEntityType::ABILITY_EFFECT)) {
            renderAbilityEffect(effect);
            m_renderedEntityCount++;
        } else {
            m_culledEntityCount++;
        }
    }
}

void WorldRenderer::renderAbilityEffect(const AbilityEffect& effect) {
    float distance = getDistanceFromCamera(effect.position);
    std::cout << "[WorldRenderer] Rendering ability effect '" << effect.abilityName 
              << "' (" << effect.effectType << ") at distance " << distance 
              << " units, time remaining: " << effect.timeRemaining << "s" << std::endl;
}

void WorldRenderer::renderParticles() {
    if (!m_settings.particlesEnabled) return;
    
    // Particle rendering would be handled here
    std::cout << "[WorldRenderer] Rendering particles (distance: " 
              << m_settings.particleRenderDistance << " units)" << std::endl;
}

void WorldRenderer::addPlayer(const RenderEntity& player) {
    m_playerQueue.push_back(player);
}

void WorldRenderer::addMonster(const RenderEntity& monster) {
    m_monsterQueue.push_back(monster);
}

void WorldRenderer::addNPC(const RenderEntity& npc) {
    m_npcQueue.push_back(npc);
}

void WorldRenderer::addProjectile(const RenderEntity& projectile) {
    m_projectileQueue.push_back(projectile);
}

void WorldRenderer::addSpellEffect(const SpellEffect& effect) {
    m_spellEffectQueue.push_back(effect);
}

void WorldRenderer::addAbilityEffect(const AbilityEffect& effect) {
    m_abilityEffectQueue.push_back(effect);
}

void WorldRenderer::removeEntity(uint32_t id, RenderEntityType type) {
    switch (type) {
        case RenderEntityType::PLAYER:
            m_playerQueue.erase(
                std::remove_if(m_playerQueue.begin(), m_playerQueue.end(),
                    [id](const RenderEntity& e) { return e.id == id; }),
                m_playerQueue.end());
            break;
        case RenderEntityType::MONSTER:
            m_monsterQueue.erase(
                std::remove_if(m_monsterQueue.begin(), m_monsterQueue.end(),
                    [id](const RenderEntity& e) { return e.id == id; }),
                m_monsterQueue.end());
            break;
        case RenderEntityType::NPC:
            m_npcQueue.erase(
                std::remove_if(m_npcQueue.begin(), m_npcQueue.end(),
                    [id](const RenderEntity& e) { return e.id == id; }),
                m_npcQueue.end());
            break;
        case RenderEntityType::PROJECTILE:
            m_projectileQueue.erase(
                std::remove_if(m_projectileQueue.begin(), m_projectileQueue.end(),
                    [id](const RenderEntity& e) { return e.id == id; }),
                m_projectileQueue.end());
            break;
        case RenderEntityType::SPELL_EFFECT:
            m_spellEffectQueue.erase(
                std::remove_if(m_spellEffectQueue.begin(), m_spellEffectQueue.end(),
                    [id](const SpellEffect& e) { return e.id == id; }),
                m_spellEffectQueue.end());
            break;
        case RenderEntityType::ABILITY_EFFECT:
            m_abilityEffectQueue.erase(
                std::remove_if(m_abilityEffectQueue.begin(), m_abilityEffectQueue.end(),
                    [id](const AbilityEffect& e) { return e.id == id; }),
                m_abilityEffectQueue.end());
            break;
        default:
            break;
    }
}

void WorldRenderer::clearAllEntities() {
    m_playerQueue.clear();
    m_monsterQueue.clear();
    m_npcQueue.clear();
    m_projectileQueue.clear();
    m_spellEffectQueue.clear();
    m_abilityEffectQueue.clear();
}

void WorldRenderer::update(float deltaTime) {
    // Update spell effect timers
    for (auto& effect : m_spellEffectQueue) {
        effect.timeRemaining -= deltaTime;
    }
    
    // Remove expired spell effects
    m_spellEffectQueue.erase(
        std::remove_if(m_spellEffectQueue.begin(), m_spellEffectQueue.end(),
            [](const SpellEffect& e) { return e.timeRemaining <= 0.0f; }),
        m_spellEffectQueue.end());
    
    // Update ability effect timers
    for (auto& effect : m_abilityEffectQueue) {
        effect.timeRemaining -= deltaTime;
    }
    
    // Remove expired ability effects
    m_abilityEffectQueue.erase(
        std::remove_if(m_abilityEffectQueue.begin(), m_abilityEffectQueue.end(),
            [](const AbilityEffect& e) { return e.timeRemaining <= 0.0f; }),
        m_abilityEffectQueue.end());
    
    // Update entity animations
    for (auto& player : m_playerQueue) {
        player.animationTime += deltaTime;
    }
    for (auto& monster : m_monsterQueue) {
        monster.animationTime += deltaTime;
    }
    for (auto& npc : m_npcQueue) {
        npc.animationTime += deltaTime;
    }
}

bool WorldRenderer::isInRenderRange(const glm::vec3& position, RenderEntityType type) const {
    return !shouldCull(position, type);
}

float WorldRenderer::getDistanceFromCamera(const glm::vec3& position) const {
    glm::vec3 diff = position - m_cameraPosition;
    return std::sqrt(diff.x * diff.x + diff.y * diff.y + diff.z * diff.z);
}

float WorldRenderer::getCurrentRenderDistance(RenderEntityType type) const {
    return getRenderDistance(type);
}

float WorldRenderer::getRenderDistance(RenderEntityType type) const {
    switch (type) {
        case RenderEntityType::PLAYER:
            return m_settings.playerRenderDistance;
        case RenderEntityType::MONSTER:
            return m_settings.monsterRenderDistance;
        case RenderEntityType::NPC:
            return m_settings.npcRenderDistance;
        case RenderEntityType::PROJECTILE:
            return m_settings.projectileRenderDistance;
        case RenderEntityType::SPELL_EFFECT:
            return m_settings.spellRenderDistance;
        case RenderEntityType::ABILITY_EFFECT:
            return m_settings.abilityRenderDistance;
        case RenderEntityType::PARTICLE:
            return m_settings.particleRenderDistance;
        case RenderEntityType::WORLD_OBJECT:
            return m_settings.worldRenderDistance;
        default:
            return m_settings.worldRenderDistance;
    }
}

bool WorldRenderer::shouldCull(const glm::vec3& position, RenderEntityType type) const {
    float distance = getDistanceFromCamera(position);
    float maxDistance = getRenderDistance(type);
    return distance > maxDistance;
}

void WorldRenderer::performFrustumCulling() {
    // Would perform frustum culling here
    // For now, just distance culling is implemented
}

// ============================================================================
// RenderEntityFactory Implementation
// ============================================================================

RenderEntity RenderEntityFactory::createPlayerEntity(uint32_t id, const std::string& name,
                                                     const glm::vec3& position, const glm::vec3& rotation) {
    RenderEntity entity;
    entity.id = id;
    entity.type = RenderEntityType::PLAYER;
    entity.position = position;
    entity.rotation = rotation;
    entity.modelName = name;
    entity.textureName = "player_default";
    entity.visible = true;
    entity.currentAnimation = "idle";
    return entity;
}

RenderEntity RenderEntityFactory::createMonsterEntity(uint32_t id, const std::string& monsterType,
                                                      const glm::vec3& position, const glm::vec3& rotation) {
    RenderEntity entity;
    entity.id = id;
    entity.type = RenderEntityType::MONSTER;
    entity.position = position;
    entity.rotation = rotation;
    entity.modelName = monsterType;
    entity.textureName = monsterType + "_texture";
    entity.visible = true;
    entity.currentAnimation = "idle";
    return entity;
}

RenderEntity RenderEntityFactory::createNPCEntity(uint32_t id, const std::string& npcType,
                                                  const glm::vec3& position, const glm::vec3& rotation) {
    RenderEntity entity;
    entity.id = id;
    entity.type = RenderEntityType::NPC;
    entity.position = position;
    entity.rotation = rotation;
    entity.modelName = npcType;
    entity.textureName = npcType + "_texture";
    entity.visible = true;
    entity.currentAnimation = "idle";
    return entity;
}

RenderEntity RenderEntityFactory::createProjectileEntity(uint32_t id, const std::string& projectileType,
                                                         const glm::vec3& position, const glm::vec3& direction) {
    RenderEntity entity;
    entity.id = id;
    entity.type = RenderEntityType::PROJECTILE;
    entity.position = position;
    entity.rotation = direction;
    entity.modelName = projectileType;
    entity.effectType = projectileType;
    entity.visible = true;
    
    // Set texture based on projectile type
    if (projectileType == "arrow") {
        entity.textureName = "arrow_texture";
    } else if (projectileType == "bullet") {
        entity.textureName = "bullet_texture";
    } else if (projectileType.find("_bolt") != std::string::npos) {
        // Magic bolt (fire_bolt, ice_bolt, etc.)
        entity.textureName = projectileType + "_texture";
        entity.effectColor = glm::vec3(1.0f, 0.5f, 0.0f); // Default orange for fire
        
        if (projectileType == "ice_bolt") {
            entity.effectColor = glm::vec3(0.5f, 0.8f, 1.0f); // Light blue
        } else if (projectileType == "arcane_bolt") {
            entity.effectColor = glm::vec3(0.8f, 0.4f, 1.0f); // Purple
        } else if (projectileType == "nature_bolt") {
            entity.effectColor = glm::vec3(0.4f, 1.0f, 0.4f); // Green
        } else if (projectileType == "acid_bolt") {
            entity.effectColor = glm::vec3(0.6f, 1.0f, 0.0f); // Yellow-green
        } else if (projectileType == "necrotic_bolt") {
            entity.effectColor = glm::vec3(0.2f, 0.1f, 0.3f); // Dark grey-purple
        } else if (projectileType == "shadow_bolt") {
            entity.effectColor = glm::vec3(0.3f, 0.0f, 0.5f); // Dark purple
        } else if (projectileType == "radiant_bolt") {
            entity.effectColor = glm::vec3(1.0f, 0.95f, 0.8f); // Warm white
        } else if (projectileType == "holy_bolt") {
            entity.effectColor = glm::vec3(1.0f, 1.0f, 0.7f); // Light yellow
        } else if (projectileType == "lightning_bolt") {
            entity.effectColor = glm::vec3(1.0f, 1.0f, 0.0f); // Bright yellow
        } else if (projectileType == "conjuring_bolt") {
            entity.effectColor = glm::vec3(0.6f, 0.3f, 0.8f); // Blue-purple
        } else if (projectileType == "psionic_bolt") {
            entity.effectColor = glm::vec3(1.0f, 0.0f, 1.0f); // Magenta
        } else if (projectileType == "water_bolt") {
            entity.effectColor = glm::vec3(0.2f, 0.5f, 1.0f); // Blue
        }
    } else {
        entity.textureName = "projectile_default";
    }
    
    return entity;
}

SpellEffect RenderEntityFactory::createSpellEffect(uint32_t id, const std::string& spellName,
                                                   const std::string& school, const glm::vec3& position,
                                                   const glm::vec3& targetPos, float duration) {
    SpellEffect effect;
    effect.id = id;
    effect.spellName = spellName;
    effect.schoolName = school;
    effect.position = position;
    effect.targetPosition = targetPos;
    effect.duration = duration;
    effect.timeRemaining = duration;
    
    // Set color based on magic school
    if (school == "fire") {
        effect.color = glm::vec3(1.0f, 0.4f, 0.0f);       // Orange
    } else if (school == "ice") {
        effect.color = glm::vec3(0.5f, 0.8f, 1.0f);       // Light blue
    } else if (school == "nature") {
        effect.color = glm::vec3(0.4f, 1.0f, 0.4f);       // Green
    } else if (school == "acid") {
        effect.color = glm::vec3(0.6f, 1.0f, 0.0f);       // Yellow-green
    } else if (school == "necrotic") {
        effect.color = glm::vec3(0.2f, 0.1f, 0.3f);       // Dark grey-purple
    } else if (school == "arcane") {
        effect.color = glm::vec3(0.8f, 0.4f, 1.0f);       // Purple
    } else if (school == "shadow") {
        effect.color = glm::vec3(0.3f, 0.0f, 0.5f);       // Dark purple
    } else if (school == "radiant") {
        effect.color = glm::vec3(1.0f, 0.95f, 0.8f);      // Warm white
    } else if (school == "holy") {
        effect.color = glm::vec3(1.0f, 1.0f, 0.7f);       // Light yellow
    } else if (school == "lightning") {
        effect.color = glm::vec3(1.0f, 1.0f, 0.0f);       // Bright yellow
    } else if (school == "conjuring") {
        effect.color = glm::vec3(0.6f, 0.3f, 0.8f);       // Blue-purple
    } else if (school == "psionic") {
        effect.color = glm::vec3(1.0f, 0.0f, 1.0f);       // Magenta
    } else if (school == "water") {
        effect.color = glm::vec3(0.2f, 0.5f, 1.0f);       // Blue
    } else {
        effect.color = glm::vec3(1.0f, 1.0f, 1.0f);       // White (default)
    }
    
    // Determine if projectile based on spell name
    effect.isProjectile = (spellName.find("bolt") != std::string::npos ||
                          spellName.find("arrow") != std::string::npos ||
                          spellName.find("missile") != std::string::npos);
    
    return effect;
}

AbilityEffect RenderEntityFactory::createAbilityEffect(uint32_t id, const std::string& abilityName,
                                                       const glm::vec3& position, const std::string& effectType,
                                                       float duration) {
    AbilityEffect effect;
    effect.id = id;
    effect.abilityName = abilityName;
    effect.position = position;
    effect.effectType = effectType;
    effect.duration = duration;
    effect.timeRemaining = duration;
    
    // Set color based on effect type
    if (effectType == "buff") {
        effect.color = glm::vec3(0.0f, 1.0f, 0.5f);
    } else if (effectType == "debuff") {
        effect.color = glm::vec3(1.0f, 0.0f, 0.5f);
    } else if (effectType == "damage") {
        effect.color = glm::vec3(1.0f, 0.0f, 0.0f);
    } else if (effectType == "heal") {
        effect.color = glm::vec3(0.0f, 1.0f, 0.0f);
    } else {
        effect.color = glm::vec3(1.0f, 1.0f, 1.0f);
    }
    
    return effect;
}

} // namespace clonemine
