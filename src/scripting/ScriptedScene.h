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
    COMPLETE_QUEST
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
    std::string stringParam;
    glm::vec3 vec3Param{0.0f};
    int intParam{0};
    float floatParam{0.0f};
    
    // For dialog actions
    std::vector<DialogLine> dialogLines;
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
}

} // namespace scripting
} // namespace clonemine
