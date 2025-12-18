#pragma once

#include <vector>
#include <unordered_map>
#include <memory>
#include <string>
#include <glm/glm.hpp>

namespace clonemine {

// Forward declarations
class World;
class Player;

/**
 * @brief Render settings that users can adjust based on their FPS
 */
struct RenderSettings {
    // Render distances (adjustable for performance)
    float worldRenderDistance{256.0f};      // Max distance to render terrain/blocks
    float playerRenderDistance{100.0f};     // Max distance to render other players
    float monsterRenderDistance{80.0f};     // Max distance to render monsters
    float spellRenderDistance{120.0f};      // Max distance to render spell effects
    float abilityRenderDistance{100.0f};    // Max distance to render ability effects
    float npcRenderDistance{80.0f};         // Max distance to render NPCs
    float projectileRenderDistance{150.0f}; // Max distance to render projectiles
    float particleRenderDistance{60.0f};    // Max distance to render particles
    
    // Detail levels (0 = low, 1 = medium, 2 = high, 3 = ultra)
    int detailLevel{2};
    
    // FPS-based quality scaling
    bool autoAdjustQuality{true};           // Automatically adjust based on FPS
    int targetFPS{60};                      // Target FPS for auto-adjustment
    int minFPS{30};                         // Minimum acceptable FPS
    
    // Shadow and lighting
    bool shadowsEnabled{true};
    float shadowDistance{64.0f};
    int shadowQuality{2};                   // 0-3
    
    // Effects
    bool particlesEnabled{true};
    bool spellEffectsEnabled{true};
    bool abilityEffectsEnabled{true};
    bool weatherEffectsEnabled{true};
    
    // Performance presets
    void setPreset(int preset) {
        switch (preset) {
            case 0: // Low
                worldRenderDistance = 128.0f;
                playerRenderDistance = 50.0f;
                monsterRenderDistance = 40.0f;
                spellRenderDistance = 60.0f;
                abilityRenderDistance = 50.0f;
                npcRenderDistance = 40.0f;
                projectileRenderDistance = 75.0f;
                particleRenderDistance = 30.0f;
                detailLevel = 0;
                shadowsEnabled = false;
                particlesEnabled = false;
                break;
            case 1: // Medium
                worldRenderDistance = 192.0f;
                playerRenderDistance = 75.0f;
                monsterRenderDistance = 60.0f;
                spellRenderDistance = 90.0f;
                abilityRenderDistance = 75.0f;
                npcRenderDistance = 60.0f;
                projectileRenderDistance = 100.0f;
                particleRenderDistance = 45.0f;
                detailLevel = 1;
                shadowsEnabled = true;
                shadowQuality = 1;
                particlesEnabled = true;
                break;
            case 2: // High
                worldRenderDistance = 256.0f;
                playerRenderDistance = 100.0f;
                monsterRenderDistance = 80.0f;
                spellRenderDistance = 120.0f;
                abilityRenderDistance = 100.0f;
                npcRenderDistance = 80.0f;
                projectileRenderDistance = 150.0f;
                particleRenderDistance = 60.0f;
                detailLevel = 2;
                shadowsEnabled = true;
                shadowQuality = 2;
                particlesEnabled = true;
                break;
            case 3: // Ultra
                worldRenderDistance = 512.0f;
                playerRenderDistance = 200.0f;
                monsterRenderDistance = 150.0f;
                spellRenderDistance = 200.0f;
                abilityRenderDistance = 150.0f;
                npcRenderDistance = 150.0f;
                projectileRenderDistance = 250.0f;
                particleRenderDistance = 100.0f;
                detailLevel = 3;
                shadowsEnabled = true;
                shadowQuality = 3;
                particlesEnabled = true;
                break;
        }
    }
};

/**
 * @brief Entity types that can be rendered
 */
enum class RenderEntityType {
    PLAYER,
    MONSTER,
    NPC,
    PROJECTILE,
    SPELL_EFFECT,
    ABILITY_EFFECT,
    PARTICLE,
    WORLD_OBJECT
};

/**
 * @brief Renderable entity data
 */
struct RenderEntity {
    uint32_t id;
    RenderEntityType type;
    glm::vec3 position;
    glm::vec3 rotation;
    glm::vec3 scale{1.0f, 1.0f, 1.0f};
    std::string modelName;
    std::string textureName;
    float animationTime{0.0f};
    std::string currentAnimation;
    bool visible{true};
    float alpha{1.0f};
    
    // For spell/ability effects
    std::string effectType;
    float effectDuration{0.0f};
    float effectTimeRemaining{0.0f};
    glm::vec3 effectColor{1.0f, 1.0f, 1.0f};
};

/**
 * @brief Active spell effect being rendered
 */
struct SpellEffect {
    uint32_t id;
    std::string spellName;
    std::string schoolName;  // fire, frost, arcane, etc.
    glm::vec3 position;
    glm::vec3 targetPosition;
    glm::vec3 color;
    float duration;
    float timeRemaining;
    float radius{1.0f};
    bool isProjectile{false};
    bool isAreaEffect{false};
};

/**
 * @brief Active ability effect being rendered
 */
struct AbilityEffect {
    uint32_t id;
    std::string abilityName;
    glm::vec3 position;
    glm::vec3 color;
    float duration;
    float timeRemaining;
    std::string effectType; // "buff", "debuff", "damage", "heal"
};

/**
 * @brief Main world renderer that handles all visual rendering
 */
class WorldRenderer {
public:
    WorldRenderer();
    ~WorldRenderer();
    
    // Initialize the renderer
    bool initialize();
    void shutdown();
    
    // Settings
    void setRenderSettings(const RenderSettings& settings);
    RenderSettings& getRenderSettings() { return m_settings; }
    const RenderSettings& getRenderSettings() const { return m_settings; }
    
    // Adjust render distance based on FPS
    void adjustRenderDistanceForFPS(float currentFPS);
    
    // Set camera/viewer position for distance culling
    void setCameraPosition(const glm::vec3& position);
    void setCameraDirection(const glm::vec3& direction);
    
    // Main render methods
    void beginFrame();
    void endFrame();
    
    // Render the world terrain/blocks
    void renderWorld(const World& world);
    
    // Render players (local and remote)
    void renderPlayers(const std::vector<RenderEntity>& players);
    void renderPlayer(const RenderEntity& player);
    
    // Render monsters
    void renderMonsters(const std::vector<RenderEntity>& monsters);
    void renderMonster(const RenderEntity& monster);
    
    // Render NPCs
    void renderNPCs(const std::vector<RenderEntity>& npcs);
    void renderNPC(const RenderEntity& npc);
    
    // Render projectiles (arrows, bullets, magic bolts)
    void renderProjectiles(const std::vector<RenderEntity>& projectiles);
    void renderProjectile(const RenderEntity& projectile);
    
    // Render spell effects
    void renderSpellEffects(const std::vector<SpellEffect>& effects);
    void renderSpellEffect(const SpellEffect& effect);
    
    // Render ability effects
    void renderAbilityEffects(const std::vector<AbilityEffect>& effects);
    void renderAbilityEffect(const AbilityEffect& effect);
    
    // Render particles
    void renderParticles();
    
    // Add entities to render queue
    void addPlayer(const RenderEntity& player);
    void addMonster(const RenderEntity& monster);
    void addNPC(const RenderEntity& npc);
    void addProjectile(const RenderEntity& projectile);
    void addSpellEffect(const SpellEffect& effect);
    void addAbilityEffect(const AbilityEffect& effect);
    
    // Remove entities
    void removeEntity(uint32_t id, RenderEntityType type);
    void clearAllEntities();
    
    // Update (for animations, effects timing)
    void update(float deltaTime);
    
    // Distance check helpers
    bool isInRenderRange(const glm::vec3& position, RenderEntityType type) const;
    float getDistanceFromCamera(const glm::vec3& position) const;
    
    // Debug/stats
    uint32_t getRenderedEntityCount() const { return m_renderedEntityCount; }
    uint32_t getCulledEntityCount() const { return m_culledEntityCount; }
    float getCurrentRenderDistance(RenderEntityType type) const;

private:
    // Render distance for entity type
    float getRenderDistance(RenderEntityType type) const;
    
    // Culling
    bool shouldCull(const glm::vec3& position, RenderEntityType type) const;
    void performFrustumCulling();
    
    // Render queues
    std::vector<RenderEntity> m_playerQueue;
    std::vector<RenderEntity> m_monsterQueue;
    std::vector<RenderEntity> m_npcQueue;
    std::vector<RenderEntity> m_projectileQueue;
    std::vector<SpellEffect> m_spellEffectQueue;
    std::vector<AbilityEffect> m_abilityEffectQueue;
    
    // Settings
    RenderSettings m_settings;
    
    // Camera
    glm::vec3 m_cameraPosition{0.0f, 0.0f, 0.0f};
    glm::vec3 m_cameraDirection{0.0f, 0.0f, -1.0f};
    
    // Stats
    uint32_t m_renderedEntityCount{0};
    uint32_t m_culledEntityCount{0};
    
    // FPS tracking for auto-adjustment
    float m_fpsHistory[60]{};
    int m_fpsHistoryIndex{0};
    float m_averageFPS{60.0f};
    
    // Initialized flag
    bool m_initialized{false};
};

/**
 * @brief Factory for creating render entities
 */
class RenderEntityFactory {
public:
    // Create player render entity
    static RenderEntity createPlayerEntity(uint32_t id, const std::string& name,
                                          const glm::vec3& position, const glm::vec3& rotation);
    
    // Create monster render entity
    static RenderEntity createMonsterEntity(uint32_t id, const std::string& monsterType,
                                           const glm::vec3& position, const glm::vec3& rotation);
    
    // Create NPC render entity
    static RenderEntity createNPCEntity(uint32_t id, const std::string& npcType,
                                       const glm::vec3& position, const glm::vec3& rotation);
    
    // Create projectile render entity
    static RenderEntity createProjectileEntity(uint32_t id, const std::string& projectileType,
                                              const glm::vec3& position, const glm::vec3& direction);
    
    // Create spell effect
    static SpellEffect createSpellEffect(uint32_t id, const std::string& spellName,
                                        const std::string& school, const glm::vec3& position,
                                        const glm::vec3& targetPos, float duration);
    
    // Create ability effect
    static AbilityEffect createAbilityEffect(uint32_t id, const std::string& abilityName,
                                            const glm::vec3& position, const std::string& effectType,
                                            float duration);
};

} // namespace clonemine
