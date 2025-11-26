#include "ScriptedScene.h"
#include "../audio/AudioManager.h"
#include <iostream>
#include <algorithm>

extern "C" {
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
}

namespace clonemine {
namespace scripting {

// ScriptedScene implementation

ScriptedScene::ScriptedScene(const std::string& name)
    : m_name(name) {
}

void ScriptedScene::addTrigger(const SceneTrigger& trigger) {
    m_triggers.push_back(trigger);
}

void ScriptedScene::addAction(const SceneAction& action) {
    m_actions.push_back(action);
}

void ScriptedScene::start() {
    if (m_playing) {
        return;
    }
    
    std::cout << "[ScriptedScene] Starting scene: " << m_name << std::endl;
    
    m_playing = true;
    m_paused = false;
    m_complete = false;
    m_currentActionIndex = 0;
    m_currentActionTime = 0.0f;
    
    if (!m_actions.empty()) {
        executeAction(m_actions[0]);
    }
}

void ScriptedScene::stop() {
    if (!m_playing) {
        return;
    }
    
    std::cout << "[ScriptedScene] Stopping scene: " << m_name << std::endl;
    
    m_playing = false;
    m_paused = false;
    m_currentActionIndex = 0;
    m_currentActionTime = 0.0f;
}

void ScriptedScene::pause() {
    if (!m_playing || m_paused) {
        return;
    }
    
    m_paused = true;
}

void ScriptedScene::resume() {
    if (!m_playing || !m_paused) {
        return;
    }
    
    m_paused = false;
}

void ScriptedScene::update(float deltaTime) {
    if (!m_playing || m_paused || m_complete) {
        return;
    }
    
    if (m_currentActionIndex >= m_actions.size()) {
        m_complete = true;
        m_playing = false;
        std::cout << "[ScriptedScene] Scene complete: " << m_name << std::endl;
        return;
    }
    
    const auto& currentAction = m_actions[m_currentActionIndex];
    m_currentActionTime += deltaTime;
    
    // Check if current action is complete
    if (m_currentActionTime >= currentAction.duration) {
        advanceToNextAction();
    }
}

float ScriptedScene::getProgress() const {
    if (m_actions.empty()) {
        return 1.0f;
    }
    
    float actionProgress = static_cast<float>(m_currentActionIndex) / static_cast<float>(m_actions.size());
    
    if (m_currentActionIndex < m_actions.size()) {
        const auto& currentAction = m_actions[m_currentActionIndex];
        if (currentAction.duration > 0.0f) {
            float subProgress = m_currentActionTime / currentAction.duration;
            actionProgress += subProgress / static_cast<float>(m_actions.size());
        }
    }
    
    return std::min(actionProgress, 1.0f);
}

bool ScriptedScene::matchesTrigger(SceneTriggerType type, const std::string& id) const {
    for (const auto& trigger : m_triggers) {
        if (trigger.type == type && trigger.id == id) {
            return true;
        }
    }
    return false;
}

void ScriptedScene::executeAction(const SceneAction& action) {
    std::cout << "[ScriptedScene] Executing action type " << static_cast<int>(action.type) << std::endl;
    
    switch (action.type) {
        case SceneActionType::DIALOG:
            for (const auto& line : action.dialogLines) {
                std::cout << "[Dialog] " << line.speakerName << ": " << line.text << std::endl;
                // TODO: Display dialog UI and play audio if available
            }
            break;
            
        case SceneActionType::PLAY_AUDIO:
            std::cout << "[ScriptedScene] Playing audio: " << action.stringParam << std::endl;
            // Audio playback handled by AudioManager
            break;
            
        case SceneActionType::MOVE_PLAYER:
            std::cout << "[ScriptedScene] Moving player to (" << action.vec3Param.x << ", " 
                      << action.vec3Param.y << ", " << action.vec3Param.z << ")" << std::endl;
            break;
            
        case SceneActionType::MOVE_CAMERA:
            std::cout << "[ScriptedScene] Moving camera" << std::endl;
            break;
            
        case SceneActionType::SHOW_TEXT:
            std::cout << "[ScriptedScene] Showing text: " << action.stringParam << std::endl;
            break;
            
        case SceneActionType::GIVE_ITEM:
            std::cout << "[ScriptedScene] Giving item: " << action.stringParam 
                      << " x" << action.intParam << std::endl;
            break;
            
        case SceneActionType::COMPLETE_QUEST:
            std::cout << "[ScriptedScene] Completing quest: " << action.stringParam << std::endl;
            break;
            
        case SceneActionType::WAIT:
            // Just wait for duration
            break;
            
        // Combat and action types
        case SceneActionType::ATTACK:
            std::cout << "[ScriptedScene] Entity '" << action.stringParam << "' attacks '" 
                      << action.stringParam2 << "'" << std::endl;
            // TODO: Trigger attack animation and damage calculation
            break;
            
        case SceneActionType::CAST_SPELL:
            std::cout << "[ScriptedScene] Entity '" << action.stringParam << "' casts spell '" 
                      << action.stringParam2 << "'" << std::endl;
            // TODO: Trigger spell animation and effects
            break;
            
        case SceneActionType::USE_ABILITY:
            std::cout << "[ScriptedScene] Entity '" << action.stringParam << "' uses ability '" 
                      << action.stringParam2 << "'" << std::endl;
            // TODO: Trigger ability effects
            break;
            
        case SceneActionType::TAKE_DAMAGE:
            std::cout << "[ScriptedScene] Entity '" << action.stringParam << "' takes " 
                      << action.intParam << " damage" << std::endl;
            // TODO: Apply damage and play hurt animation
            break;
            
        case SceneActionType::DIE:
            std::cout << "[ScriptedScene] Entity '" << action.stringParam << "' dies (" 
                      << action.stringParam2 << ")" << std::endl;
            // TODO: Trigger death animation and effects
            break;
            
        case SceneActionType::SHAPE_CHANGE:
            std::cout << "[ScriptedScene] Entity '" << action.stringParam 
                      << "' transforms into '" << action.stringParam2 << "'" << std::endl;
            // TODO: Trigger transformation animation
            break;
            
        case SceneActionType::APPLY_STATUS:
            std::cout << "[ScriptedScene] Applying status '" << action.stringParam2 
                      << "' to entity '" << action.stringParam << "'" << std::endl;
            // TODO: Apply status effect
            break;
            
        case SceneActionType::REMOVE_STATUS:
            std::cout << "[ScriptedScene] Removing status '" << action.stringParam2 
                      << "' from entity '" << action.stringParam << "'" << std::endl;
            // TODO: Remove status effect
            break;
            
        case SceneActionType::AREA_EFFECT:
            std::cout << "[ScriptedScene] Area effect '" << action.stringParam << "' at (" 
                      << action.vec3Param.x << ", " << action.vec3Param.y << ", " 
                      << action.vec3Param.z << ") radius: " << action.floatParam << std::endl;
            // TODO: Create area effect (explosion, disintegration zone, etc.)
            break;
            
        case SceneActionType::DESTROY_ENTITY:
            std::cout << "[ScriptedScene] Destroying entity '" << action.stringParam 
                      << "' via " << action.stringParam2 << std::endl;
            // TODO: Trigger destruction effect (vaporize, disintegrate, etc.)
            break;
            
        case SceneActionType::HEAL:
            std::cout << "[ScriptedScene] Healing entity '" << action.stringParam 
                      << "' for " << action.intParam << " HP" << std::endl;
            // TODO: Apply healing
            break;
            
        case SceneActionType::TELEPORT:
            std::cout << "[ScriptedScene] Teleporting entity '" << action.stringParam << "' to (" 
                      << action.vec3Param.x << ", " << action.vec3Param.y << ", " 
                      << action.vec3Param.z << ")" << std::endl;
            // TODO: Teleport entity
            break;
            
        case SceneActionType::KNOCKBACK:
            std::cout << "[ScriptedScene] Knocking back entity '" << action.stringParam 
                      << "' in direction (" << action.vec3Param.x << ", " 
                      << action.vec3Param.y << ", " << action.vec3Param.z 
                      << ") with force " << action.floatParam << std::endl;
            // TODO: Apply knockback force
            break;
            
        case SceneActionType::CHAIN_ACTIONS:
            std::cout << "[ScriptedScene] Executing " << action.chainedActions.size() 
                      << " chained actions" << std::endl;
            // Execute all chained actions in sequence
            for (const auto& chainedAction : action.chainedActions) {
                executeAction(chainedAction);
            }
            break;
            
        default:
            break;
    }
}

void ScriptedScene::advanceToNextAction() {
    m_currentActionIndex++;
    m_currentActionTime = 0.0f;
    
    if (m_currentActionIndex < m_actions.size()) {
        executeAction(m_actions[m_currentActionIndex]);
    }
}

// SceneManager implementation

SceneManager::SceneManager() = default;

SceneManager::~SceneManager() {
    shutdown();
}

bool SceneManager::initialize(audio::AudioManager* audioManager) {
    if (m_initialized) {
        return true;
    }
    
    std::cout << "[SceneManager] Initializing scene manager..." << std::endl;
    
    m_audioManager = audioManager;
    
    // Initialize Lua state
    m_luaState = luaL_newstate();
    if (!m_luaState) {
        std::cerr << "[SceneManager] Failed to create Lua state" << std::endl;
        return false;
    }
    
    luaL_openlibs(m_luaState);
    initializeLuaBindings();
    
    m_initialized = true;
    std::cout << "[SceneManager] Scene manager initialized successfully" << std::endl;
    return true;
}

void SceneManager::shutdown() {
    if (!m_initialized) {
        return;
    }
    
    std::cout << "[SceneManager] Shutting down scene manager..." << std::endl;
    
    stopAllScenes();
    unloadAllScenes();
    cleanupLuaState();
    
    m_initialized = false;
}

bool SceneManager::loadScene(const std::string& filepath) {
    std::cout << "[SceneManager] Loading scene from file: " << filepath << std::endl;
    
    // TODO: Load and execute Lua script file
    // if (luaL_dofile(m_luaState, filepath.c_str()) != LUA_OK) {
    //     std::cerr << "[SceneManager] Error loading scene: " << lua_tostring(m_luaState, -1) << std::endl;
    //     lua_pop(m_luaState, 1);
    //     return false;
    // }
    
    return true;
}

bool SceneManager::loadSceneFromLua(const std::string& luaScript) {
    std::cout << "[SceneManager] Loading scene from Lua script" << std::endl;
    
    if (luaL_dostring(m_luaState, luaScript.c_str()) != LUA_OK) {
        std::cerr << "[SceneManager] Error executing Lua script: " << lua_tostring(m_luaState, -1) << std::endl;
        lua_pop(m_luaState, 1);
        return false;
    }
    
    return true;
}

void SceneManager::unloadScene(const std::string& name) {
    auto it = m_scenes.find(name);
    if (it != m_scenes.end()) {
        stopScene(name);
        m_scenes.erase(it);
    }
}

void SceneManager::unloadAllScenes() {
    stopAllScenes();
    m_scenes.clear();
}

void SceneManager::playScene(const std::string& name) {
    auto it = m_scenes.find(name);
    if (it != m_scenes.end()) {
        it->second->start();
        
        // Add to playing scenes if not already there
        auto playingIt = std::find(m_playingScenes.begin(), m_playingScenes.end(), it->second);
        if (playingIt == m_playingScenes.end()) {
            m_playingScenes.push_back(it->second);
        }
    }
}

void SceneManager::stopScene(const std::string& name) {
    auto it = m_scenes.find(name);
    if (it != m_scenes.end()) {
        it->second->stop();
        
        // Remove from playing scenes
        auto playingIt = std::find(m_playingScenes.begin(), m_playingScenes.end(), it->second);
        if (playingIt != m_playingScenes.end()) {
            m_playingScenes.erase(playingIt);
        }
    }
}

void SceneManager::stopAllScenes() {
    for (auto& scene : m_playingScenes) {
        scene->stop();
    }
    m_playingScenes.clear();
}

void SceneManager::triggerScenes(SceneTriggerType type, const std::string& id) {
    std::cout << "[SceneManager] Triggering scenes for type " << static_cast<int>(type) 
              << " with id: " << id << std::endl;
    
    for (auto& [name, scene] : m_scenes) {
        if (scene->matchesTrigger(type, id) && !scene->isPlaying()) {
            playScene(name);
        }
    }
}

void SceneManager::update(float deltaTime) {
    // Update all playing scenes
    auto it = m_playingScenes.begin();
    while (it != m_playingScenes.end()) {
        (*it)->update(deltaTime);
        
        // Remove completed scenes
        if ((*it)->isComplete()) {
            it = m_playingScenes.erase(it);
        } else {
            ++it;
        }
    }
}

std::shared_ptr<ScriptedScene> SceneManager::createScene(const std::string& name) {
    auto scene = std::make_shared<ScriptedScene>(name);
    m_scenes[name] = scene;
    return scene;
}

void SceneManager::addSceneTrigger(const std::string& sceneName, SceneTriggerType type, const std::string& id) {
    auto it = m_scenes.find(sceneName);
    if (it != m_scenes.end()) {
        SceneTrigger trigger;
        trigger.type = type;
        trigger.id = id;
        it->second->addTrigger(trigger);
    }
}

void SceneManager::addSceneAction(const std::string& sceneName, const SceneAction& action) {
    auto it = m_scenes.find(sceneName);
    if (it != m_scenes.end()) {
        it->second->addAction(action);
    }
}

bool SceneManager::hasScene(const std::string& name) const {
    return m_scenes.find(name) != m_scenes.end();
}

bool SceneManager::isScenePlaying(const std::string& name) const {
    auto it = m_scenes.find(name);
    return it != m_scenes.end() && it->second->isPlaying();
}

std::shared_ptr<ScriptedScene> SceneManager::getScene(const std::string& name) {
    auto it = m_scenes.find(name);
    return (it != m_scenes.end()) ? it->second : nullptr;
}

void SceneManager::initializeLuaBindings() {
    // TODO: Register C++ functions with Lua for scene creation
    // This would allow Lua scripts to call functions like:
    // createScene("scene_name")
    // addDialog("scene_name", "Speaker", "Text")
    // addTrigger("scene_name", "QUEST_START", "quest_id")
}

void SceneManager::cleanupLuaState() {
    if (m_luaState) {
        lua_close(m_luaState);
        m_luaState = nullptr;
    }
}

// SceneBuilder helper functions

namespace SceneBuilder {

SceneAction createDialogAction(const std::string& speaker, const std::string& text, 
                               const std::string& audioFile, float duration) {
    SceneAction action;
    action.type = SceneActionType::DIALOG;
    action.duration = duration;
    
    DialogLine line;
    line.speakerName = speaker;
    line.text = text;
    line.audioFile = audioFile;
    line.displayTime = duration;
    
    action.dialogLines.push_back(line);
    return action;
}

SceneAction createMovePlayerAction(const glm::vec3& position, float duration) {
    SceneAction action;
    action.type = SceneActionType::MOVE_PLAYER;
    action.vec3Param = position;
    action.duration = duration;
    return action;
}

SceneAction createMoveCameraAction(const glm::vec3& position, const glm::vec3& lookAt, float duration) {
    (void)lookAt; // TODO: Store in additional parameter when needed
    SceneAction action;
    action.type = SceneActionType::MOVE_CAMERA;
    action.vec3Param = position;
    action.duration = duration;
    return action;
}

SceneAction createPlayAudioAction(const std::string& audioName) {
    SceneAction action;
    action.type = SceneActionType::PLAY_AUDIO;
    action.stringParam = audioName;
    action.duration = 0.1f; // Minimal duration for instant actions
    return action;
}

SceneAction createWaitAction(float duration) {
    SceneAction action;
    action.type = SceneActionType::WAIT;
    action.duration = duration;
    return action;
}

SceneAction createFadeAction(bool fadeIn, float duration) {
    SceneAction action;
    action.type = fadeIn ? SceneActionType::FADE_IN : SceneActionType::FADE_OUT;
    action.duration = duration;
    return action;
}

SceneAction createShowTextAction(const std::string& text, float duration) {
    SceneAction action;
    action.type = SceneActionType::SHOW_TEXT;
    action.stringParam = text;
    action.duration = duration;
    return action;
}

SceneAction createGiveItemAction(const std::string& itemId, int quantity) {
    SceneAction action;
    action.type = SceneActionType::GIVE_ITEM;
    action.stringParam = itemId;
    action.intParam = quantity;
    action.duration = 0.1f;
    return action;
}

SceneAction createCompleteQuestAction(const std::string& questId) {
    SceneAction action;
    action.type = SceneActionType::COMPLETE_QUEST;
    action.stringParam = questId;
    action.duration = 0.1f;
    return action;
}

// Combat and action helpers

SceneAction createAttackAction(const std::string& entityId, const std::string& targetId, 
                               const std::string& attackType, float duration) {
    SceneAction action;
    action.type = SceneActionType::ATTACK;
    action.stringParam = entityId;
    action.stringParam2 = targetId;
    action.duration = duration;
    return action;
}

SceneAction createCastSpellAction(const std::string& entityId, const std::string& spellName,
                                 const std::string& targetId, const glm::vec3& targetPos) {
    SceneAction action;
    action.type = SceneActionType::CAST_SPELL;
    action.stringParam = entityId;
    action.stringParam2 = spellName;
    action.vec3Param = targetPos;
    action.duration = 2.0f; // Default spell cast time
    return action;
}

SceneAction createUseAbilityAction(const std::string& entityId, const std::string& abilityName,
                                   const std::string& targetId) {
    SceneAction action;
    action.type = SceneActionType::USE_ABILITY;
    action.stringParam = entityId;
    action.stringParam2 = abilityName;
    action.duration = 1.0f;
    return action;
}

SceneAction createTakeDamageAction(const std::string& entityId, int damageAmount, 
                                   const std::string& damageType) {
    SceneAction action;
    action.type = SceneActionType::TAKE_DAMAGE;
    action.stringParam = entityId;
    action.stringParam2 = damageType;
    action.intParam = damageAmount;
    action.duration = 0.5f; // Time for hurt animation
    return action;
}

SceneAction createDieAction(const std::string& entityId, const std::string& deathType) {
    SceneAction action;
    action.type = SceneActionType::DIE;
    action.stringParam = entityId;
    action.stringParam2 = deathType; // normal, vaporize, disintegrate, explode, etc.
    action.duration = 2.0f; // Time for death animation
    return action;
}

SceneAction createShapeChangeAction(const std::string& entityId, const std::string& newForm,
                                    float duration) {
    SceneAction action;
    action.type = SceneActionType::SHAPE_CHANGE;
    action.stringParam = entityId;
    action.stringParam2 = newForm;
    action.duration = duration;
    return action;
}

SceneAction createApplyStatusAction(const std::string& entityId, const std::string& statusName,
                                    float duration) {
    SceneAction action;
    action.type = SceneActionType::APPLY_STATUS;
    action.stringParam = entityId;
    action.stringParam2 = statusName; // poison, stun, slow, haste, etc.
    action.floatParam = duration; // Status effect duration
    action.duration = 0.5f; // Action execution time
    return action;
}

SceneAction createAreaEffectAction(const std::string& effectType, const glm::vec3& position,
                                   float radius, int power) {
    SceneAction action;
    action.type = SceneActionType::AREA_EFFECT;
    action.stringParam = effectType; // explosion, disintegration, freeze, burn, etc.
    action.vec3Param = position;
    action.floatParam = radius;
    action.intParam = power;
    action.duration = 3.0f; // Effect duration
    return action;
}

SceneAction createDestroyEntityAction(const std::string& entityId, 
                                      const std::string& destructionType) {
    SceneAction action;
    action.type = SceneActionType::DESTROY_ENTITY;
    action.stringParam = entityId;
    action.stringParam2 = destructionType; // vaporize, disintegrate, crumble, explode
    action.duration = 1.0f; // Destruction effect duration
    return action;
}

SceneAction createHealAction(const std::string& entityId, int healAmount) {
    SceneAction action;
    action.type = SceneActionType::HEAL;
    action.stringParam = entityId;
    action.intParam = healAmount;
    action.duration = 1.0f; // Healing effect duration
    return action;
}

SceneAction createTeleportAction(const std::string& entityId, const glm::vec3& position) {
    SceneAction action;
    action.type = SceneActionType::TELEPORT;
    action.stringParam = entityId;
    action.vec3Param = position;
    action.duration = 0.5f; // Teleport effect duration
    return action;
}

SceneAction createKnockbackAction(const std::string& entityId, const glm::vec3& direction,
                                  float force) {
    SceneAction action;
    action.type = SceneActionType::KNOCKBACK;
    action.stringParam = entityId;
    action.vec3Param = direction;
    action.floatParam = force;
    action.duration = 1.0f; // Knockback animation duration
    return action;
}

SceneAction createChainedAction(const std::vector<SceneAction>& actions) {
    SceneAction action;
    action.type = SceneActionType::CHAIN_ACTIONS;
    action.chainedActions = actions;
    
    // Calculate total duration from all chained actions
    float totalDuration = 0.0f;
    for (const auto& chainedAction : actions) {
        totalDuration += chainedAction.duration;
    }
    action.duration = totalDuration;
    
    return action;
}

} // namespace SceneBuilder

} // namespace scripting
} // namespace clonemine
