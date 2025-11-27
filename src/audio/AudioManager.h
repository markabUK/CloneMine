#pragma once

#include <string>
#include <unordered_map>
#include <memory>
#include <glm/glm.hpp>

// Forward declarations for OpenAL
typedef struct ALCdevice_struct ALCdevice;
typedef struct ALCcontext_struct ALCcontext;

namespace clonemine {
namespace audio {

enum class AudioCategory {
    WEAPON,
    SPELL,
    CREATURE,
    PLAYER,
    DIALOG,
    AMBIENT,
    MUSIC,
    UI
};

enum class AudioEvent {
    // Weapon events
    SWORD_SWING,
    SWORD_HIT,
    BOW_SHOOT,
    ARROW_HIT,
    
    // Spell events
    FIREBALL_CAST,
    FIREBALL_IMPACT,
    HEAL_CAST,
    LIGHTNING_CAST,
    
    // Creature events
    ZOMBIE_IDLE,
    ZOMBIE_ATTACK,
    ZOMBIE_DEATH,
    SKELETON_IDLE,
    SKELETON_DEATH,
    
    // Player events
    PLAYER_HURT,
    PLAYER_DEATH,
    PLAYER_LEVEL_UP,
    FOOTSTEP,
    JUMP,
    LAND,
    
    // Dialog/UI
    DIALOG_OPEN,
    DIALOG_CLOSE,
    QUEST_ACCEPT,
    QUEST_COMPLETE
};

struct AudioSource {
    unsigned int sourceId{0};
    unsigned int bufferId{0};
    AudioCategory category;
    bool is3D{true};
    float volume{1.0f};
    float pitch{1.0f};
    bool looping{false};
    
    glm::vec3 position{0.0f};
    glm::vec3 velocity{0.0f};
};

class AudioManager {
public:
    AudioManager();
    ~AudioManager();
    
    // Disable copy
    AudioManager(const AudioManager&) = delete;
    AudioManager& operator=(const AudioManager&) = delete;
    
    // Initialize/shutdown
    bool initialize();
    void shutdown();
    
    // Audio loading
    bool loadSound(const std::string& name, const std::string& filepath, AudioCategory category);
    bool loadMusic(const std::string& name, const std::string& filepath);
    void unloadSound(const std::string& name);
    
    // Play sounds
    void playSound(const std::string& name, float volume = 1.0f);
    void playSound3D(const std::string& name, const glm::vec3& position, float volume = 1.0f);
    void playEvent(AudioEvent event, const glm::vec3& position = glm::vec3(0.0f));
    
    // Music control
    void playMusic(const std::string& name, bool loop = true);
    void stopMusic();
    void pauseMusic();
    void resumeMusic();
    
    // Volume control
    void setMasterVolume(float volume);
    void setCategoryVolume(AudioCategory category, float volume);
    
    // Listener (player) position for 3D audio
    void setListenerPosition(const glm::vec3& position);
    void setListenerVelocity(const glm::vec3& velocity);
    void setListenerOrientation(const glm::vec3& forward, const glm::vec3& up);
    
    // Update (call every frame)
    void update(float deltaTime);
    
    // Query state
    [[nodiscard]] bool isInitialized() const { return m_initialized; }
    [[nodiscard]] float getMasterVolume() const { return m_masterVolume; }
    
private:
    bool m_initialized{false};
    
    // OpenAL context
    ALCdevice* m_device{nullptr};
    ALCcontext* m_context{nullptr};
    
    // Audio sources and buffers
    std::unordered_map<std::string, std::shared_ptr<AudioSource>> m_sounds;
    std::unordered_map<AudioEvent, std::string> m_eventMap;
    
    // Current music
    std::string m_currentMusic;
    unsigned int m_musicSourceId{0};
    
    // Volume settings
    float m_masterVolume{1.0f};
    std::unordered_map<AudioCategory, float> m_categoryVolumes;
    
    // Listener state
    glm::vec3 m_listenerPosition{0.0f};
    glm::vec3 m_listenerVelocity{0.0f};
    glm::vec3 m_listenerForward{0.0f, 0.0f, -1.0f};
    glm::vec3 m_listenerUp{0.0f, 1.0f, 0.0f};
    
    // Helper methods
    unsigned int createSource();
    unsigned int loadAudioFile(const std::string& filepath);
    void deleteSource(unsigned int sourceId);
    void deleteBuffer(unsigned int bufferId);
    void initializeEventMap();
    float getCategoryVolume(AudioCategory category) const;
};

} // namespace audio
} // namespace clonemine
