#pragma once

#include <string>
#include <vector>
#include <memory>
#include <functional>
#include <glm/glm.hpp>

// Forward declarations
struct lua_State;

namespace clonemine {

namespace audio {
class AudioManager;
}

namespace scripting {

enum class SceneTriggerType {
    QUEST_START,
    QUEST_COMPLETE,
    QUEST_PROGRESS,
    NPC_INTERACTION,
    LOCATION_ENTER,
    ITEM_PICKUP,
    ENEMY_DEFEAT,
    LEVEL_UP,
    MANUAL
};

enum class SceneActionType {
    DIALOG,
    MOVE_PLAYER,
    MOVE_CAMERA,
    PLAY_ANIMATION,
    PLAY_AUDIO,
    SPAWN_ENTITY,
    WAIT,
    FADE_IN,
    FADE_OUT,
    SET_CAMERA_TARGET,
    SHOW_TEXT,
    GIVE_ITEM,
    COMPLETE_QUEST,
    // Combat and action types
    ATTACK,              // Entity performs attack action
    CAST_SPELL,          // Entity casts a spell
    USE_ABILITY,         // Entity uses an ability
    TAKE_DAMAGE,         // Entity takes damage
    DIE,                 // Entity dies (triggers death animation/effects)
    SHAPE_CHANGE,        // Entity transforms into another form
    APPLY_STATUS,        // Apply status effect (buff/debuff)
    REMOVE_STATUS,       // Remove status effect
    AREA_EFFECT,         // Create area effect (explosion, disintegration, etc.)
    DESTROY_ENTITY,      // Instantly destroy/vaporize entity
    HEAL,                // Heal entity
    TELEPORT,            // Teleport entity to position
    KNOCKBACK,           // Apply knockback force to entity
    CHAIN_ACTIONS        // Execute multiple actions in sequence on same entity
};

struct DialogLine {
    std::string speakerName;
    std::string text;
    std::string audioFile;  // Optional dialog audio
    float displayTime{5.0f};  // How long to show the text
};

struct SceneAction {
    SceneActionType type;
    float duration{0.0f};
    std::string stringParam;      // Entity ID, spell name, ability name, etc.
    std::string stringParam2;     // Target entity ID, effect type, etc.
    glm::vec3 vec3Param{0.0f};    // Position, direction, color
    glm::vec3 vec3Param2{0.0f};   // Additional position/vector data
    int intParam{0};              // Damage amount, status ID, etc.
    float floatParam{0.0f};       // Radius, intensity, scale, etc.
    float floatParam2{0.0f};      // Additional float parameter
    bool boolParam{false};        // Flags (instant effect, etc.)
    
    // For dialog actions
    std::vector<DialogLine> dialogLines;
    
    // For chained actions
    std::vector<SceneAction> chainedActions;
};

struct SceneTrigger {
    SceneTriggerType type;
    std::string id;  // Quest ID, NPC name, location name, etc.
    std::string conditionScript;  // Optional Lua script for conditions
};

class ScriptedScene {
public:
    ScriptedScene(const std::string& name);
    ~ScriptedScene() = default;
    
    // Scene definition
    void setName(const std::string& name) { m_name = name; }
    void setDescription(const std::string& desc) { m_description = desc; }
    void addTrigger(const SceneTrigger& trigger);
    void addAction(const SceneAction& action);
    
    // Scene control
    void start();
    void stop();
    void pause();
    void resume();
    void update(float deltaTime);
    
    // State queries
    [[nodiscard]] bool isPlaying() const { return m_playing; }
    [[nodiscard]] bool isPaused() const { return m_paused; }
    [[nodiscard]] bool isComplete() const { return m_complete; }
    [[nodiscard]] const std::string& getName() const { return m_name; }
    [[nodiscard]] float getProgress() const;
    
    // Trigger checking
    [[nodiscard]] bool matchesTrigger(SceneTriggerType type, const std::string& id) const;
    
private:
    std::string m_name;
    std::string m_description;
    std::vector<SceneTrigger> m_triggers;
    std::vector<SceneAction> m_actions;
    
    bool m_playing{false};
    bool m_paused{false};
    bool m_complete{false};
    
    size_t m_currentActionIndex{0};
    float m_currentActionTime{0.0f};
    
    void executeAction(const SceneAction& action);
    void advanceToNextAction();
};

class SceneManager {
public:
    SceneManager();
    ~SceneManager();
    
    // Disable copy
    SceneManager(const SceneManager&) = delete;
    SceneManager& operator=(const SceneManager&) = delete;
    
    // Initialize/shutdown
    bool initialize(audio::AudioManager* audioManager);
    void shutdown();
    
    // Scene loading
    bool loadScene(const std::string& filepath);
    bool loadSceneFromLua(const std::string& luaScript);
    void unloadScene(const std::string& name);
    void unloadAllScenes();
    
    // Scene playback
    void playScene(const std::string& name);
    void stopScene(const std::string& name);
    void stopAllScenes();
    
    // Trigger handling
    void triggerScenes(SceneTriggerType type, const std::string& id);
    
    // Update
    void update(float deltaTime);
    
    // Scene creation API (for Lua scripts)
    std::shared_ptr<ScriptedScene> createScene(const std::string& name);
    void addSceneTrigger(const std::string& sceneName, SceneTriggerType type, const std::string& id);
    void addSceneAction(const std::string& sceneName, const SceneAction& action);
    
    // Query
    [[nodiscard]] bool hasScene(const std::string& name) const;
    [[nodiscard]] bool isScenePlaying(const std::string& name) const;
    [[nodiscard]] std::shared_ptr<ScriptedScene> getScene(const std::string& name);
    
    // Audio manager access (for scene actions)
    [[nodiscard]] audio::AudioManager* getAudioManager() { return m_audioManager; }
    
private:
    bool m_initialized{false};
    audio::AudioManager* m_audioManager{nullptr};
    
    std::unordered_map<std::string, std::shared_ptr<ScriptedScene>> m_scenes;
    std::vector<std::shared_ptr<ScriptedScene>> m_playingScenes;
    
    // Lua state for scene scripts
    lua_State* m_luaState{nullptr};
    
    void initializeLuaBindings();
    void cleanupLuaState();
};

// Helper functions for creating scenes
namespace SceneBuilder {
    SceneAction createDialogAction(const std::string& speaker, const std::string& text, 
                                   const std::string& audioFile = "", float duration = 5.0f);
    SceneAction createMovePlayerAction(const glm::vec3& position, float duration);
    SceneAction createMoveCameraAction(const glm::vec3& position, const glm::vec3& lookAt, float duration);
    SceneAction createPlayAudioAction(const std::string& audioName);
    SceneAction createWaitAction(float duration);
    SceneAction createFadeAction(bool fadeIn, float duration);
    SceneAction createShowTextAction(const std::string& text, float duration);
    SceneAction createGiveItemAction(const std::string& itemId, int quantity);
    SceneAction createCompleteQuestAction(const std::string& questId);
    
    // Combat and action helpers
    SceneAction createAttackAction(const std::string& entityId, const std::string& targetId, 
                                   const std::string& attackType = "melee", float duration = 1.0f);
    SceneAction createCastSpellAction(const std::string& entityId, const std::string& spellName,
                                     const std::string& targetId = "", const glm::vec3& targetPos = glm::vec3(0.0f));
    SceneAction createUseAbilityAction(const std::string& entityId, const std::string& abilityName,
                                       const std::string& targetId = "");
    SceneAction createTakeDamageAction(const std::string& entityId, int damageAmount, 
                                       const std::string& damageType = "physical");
    SceneAction createDieAction(const std::string& entityId, const std::string& deathType = "normal");
    SceneAction createShapeChangeAction(const std::string& entityId, const std::string& newForm,
                                        float duration = 2.0f);
    SceneAction createApplyStatusAction(const std::string& entityId, const std::string& statusName,
                                        float duration = 10.0f);
    SceneAction createAreaEffectAction(const std::string& effectType, const glm::vec3& position,
                                       float radius, int power = 100);
    SceneAction createDestroyEntityAction(const std::string& entityId, 
                                          const std::string& destructionType = "vaporize");
    SceneAction createHealAction(const std::string& entityId, int healAmount);
    SceneAction createTeleportAction(const std::string& entityId, const glm::vec3& position);
    SceneAction createKnockbackAction(const std::string& entityId, const glm::vec3& direction,
                                      float force);
    SceneAction createChainedAction(const std::vector<SceneAction>& actions);
}

} // namespace scripting
} // namespace clonemine
