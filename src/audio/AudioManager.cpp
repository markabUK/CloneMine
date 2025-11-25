#include "AudioManager.h"
#include <iostream>
#include <cstring>

// OpenAL includes - will be added via CMake
// For now, we'll create stubs that can be replaced with real OpenAL implementation
// #include <AL/al.h>
// #include <AL/alc.h>

namespace clonemine {
namespace audio {

AudioManager::AudioManager() {
    // Initialize category volumes to default
    m_categoryVolumes[AudioCategory::WEAPON] = 1.0f;
    m_categoryVolumes[AudioCategory::SPELL] = 1.0f;
    m_categoryVolumes[AudioCategory::CREATURE] = 1.0f;
    m_categoryVolumes[AudioCategory::PLAYER] = 1.0f;
    m_categoryVolumes[AudioCategory::DIALOG] = 1.0f;
    m_categoryVolumes[AudioCategory::AMBIENT] = 0.5f;
    m_categoryVolumes[AudioCategory::MUSIC] = 0.7f;
    m_categoryVolumes[AudioCategory::UI] = 0.8f;
}

AudioManager::~AudioManager() {
    shutdown();
}

bool AudioManager::initialize() {
    if (m_initialized) {
        return true;
    }
    
    std::cout << "[AudioManager] Initializing audio system..." << std::endl;
    
    // TODO: Initialize OpenAL
    // m_device = alcOpenDevice(nullptr);
    // if (!m_device) {
    //     std::cerr << "[AudioManager] Failed to open audio device" << std::endl;
    //     return false;
    // }
    
    // m_context = alcCreateContext(m_device, nullptr);
    // if (!m_context) {
    //     std::cerr << "[AudioManager] Failed to create audio context" << std::endl;
    //     alcCloseDevice(m_device);
    //     m_device = nullptr;
    //     return false;
    // }
    
    // alcMakeContextCurrent(m_context);
    
    // Initialize event mappings
    initializeEventMap();
    
    m_initialized = true;
    std::cout << "[AudioManager] Audio system initialized successfully" << std::endl;
    return true;
}

void AudioManager::shutdown() {
    if (!m_initialized) {
        return;
    }
    
    std::cout << "[AudioManager] Shutting down audio system..." << std::endl;
    
    // Clean up all sounds
    m_sounds.clear();
    
    // TODO: Shutdown OpenAL
    // if (m_context) {
    //     alcMakeContextCurrent(nullptr);
    //     alcDestroyContext(m_context);
    //     m_context = nullptr;
    // }
    
    // if (m_device) {
    //     alcCloseDevice(m_device);
    //     m_device = nullptr;
    // }
    
    m_initialized = false;
}

bool AudioManager::loadSound(const std::string& name, const std::string& filepath, AudioCategory category) {
    std::cout << "[AudioManager] Loading sound: " << name << " from " << filepath << std::endl;
    
    // TODO: Implement actual audio file loading
    // unsigned int bufferId = loadAudioFile(filepath);
    // if (bufferId == 0) {
    //     return false;
    // }
    
    auto source = std::make_shared<AudioSource>();
    // source->bufferId = bufferId;
    source->category = category;
    source->volume = 1.0f;
    
    m_sounds[name] = source;
    return true;
}

bool AudioManager::loadMusic(const std::string& name, const std::string& filepath) {
    return loadSound(name, filepath, AudioCategory::MUSIC);
}

void AudioManager::unloadSound(const std::string& name) {
    auto it = m_sounds.find(name);
    if (it != m_sounds.end()) {
        m_sounds.erase(it);
    }
}

void AudioManager::playSound(const std::string& name, float volume) {
    auto it = m_sounds.find(name);
    if (it == m_sounds.end()) {
        return;
    }
    
    std::cout << "[AudioManager] Playing sound: " << name << " at volume " << volume << std::endl;
    
    // TODO: Implement actual audio playback
    // unsigned int sourceId = createSource();
    // alSourcei(sourceId, AL_BUFFER, it->second->bufferId);
    // float finalVolume = volume * getCategoryVolume(it->second->category) * m_masterVolume;
    // alSourcef(sourceId, AL_GAIN, finalVolume);
    // alSourcePlay(sourceId);
}

void AudioManager::playSound3D(const std::string& name, const glm::vec3& position, float volume) {
    auto it = m_sounds.find(name);
    if (it == m_sounds.end()) {
        return;
    }
    
    (void)volume; // TODO: Use when OpenAL is integrated
    
    std::cout << "[AudioManager] Playing 3D sound: " << name << " at position (" 
              << position.x << ", " << position.y << ", " << position.z << ")" << std::endl;
    
    // TODO: Implement 3D audio playback
    // unsigned int sourceId = createSource();
    // alSourcei(sourceId, AL_BUFFER, it->second->bufferId);
    // alSource3f(sourceId, AL_POSITION, position.x, position.y, position.z);
    // alSourcei(sourceId, AL_SOURCE_RELATIVE, AL_FALSE);
    // float finalVolume = volume * getCategoryVolume(it->second->category) * m_masterVolume;
    // alSourcef(sourceId, AL_GAIN, finalVolume);
    // alSourcePlay(sourceId);
}

void AudioManager::playEvent(AudioEvent event, const glm::vec3& position) {
    auto it = m_eventMap.find(event);
    if (it != m_eventMap.end()) {
        if (position == glm::vec3(0.0f)) {
            playSound(it->second);
        } else {
            playSound3D(it->second, position);
        }
    }
}

void AudioManager::playMusic(const std::string& name, bool loop) {
    stopMusic();
    
    auto it = m_sounds.find(name);
    if (it == m_sounds.end()) {
        return;
    }
    
    std::cout << "[AudioManager] Playing music: " << name << (loop ? " (looping)" : "") << std::endl;
    
    m_currentMusic = name;
    
    // TODO: Implement music playback
    // m_musicSourceId = createSource();
    // alSourcei(m_musicSourceId, AL_BUFFER, it->second->bufferId);
    // alSourcei(m_musicSourceId, AL_LOOPING, loop ? AL_TRUE : AL_FALSE);
    // float volume = getCategoryVolume(AudioCategory::MUSIC) * m_masterVolume;
    // alSourcef(m_musicSourceId, AL_GAIN, volume);
    // alSourcePlay(m_musicSourceId);
}

void AudioManager::stopMusic() {
    if (m_musicSourceId != 0) {
        // TODO: Stop music
        // alSourceStop(m_musicSourceId);
        // deleteSource(m_musicSourceId);
        m_musicSourceId = 0;
        m_currentMusic.clear();
    }
}

void AudioManager::pauseMusic() {
    if (m_musicSourceId != 0) {
        // TODO: Pause music
        // alSourcePause(m_musicSourceId);
    }
}

void AudioManager::resumeMusic() {
    if (m_musicSourceId != 0) {
        // TODO: Resume music
        // alSourcePlay(m_musicSourceId);
    }
}

void AudioManager::setMasterVolume(float volume) {
    m_masterVolume = glm::clamp(volume, 0.0f, 1.0f);
    std::cout << "[AudioManager] Master volume set to " << m_masterVolume << std::endl;
}

void AudioManager::setCategoryVolume(AudioCategory category, float volume) {
    m_categoryVolumes[category] = glm::clamp(volume, 0.0f, 1.0f);
}

void AudioManager::setListenerPosition(const glm::vec3& position) {
    m_listenerPosition = position;
    // TODO: Update OpenAL listener
    // alListener3f(AL_POSITION, position.x, position.y, position.z);
}

void AudioManager::setListenerVelocity(const glm::vec3& velocity) {
    m_listenerVelocity = velocity;
    // TODO: Update OpenAL listener
    // alListener3f(AL_VELOCITY, velocity.x, velocity.y, velocity.z);
}

void AudioManager::setListenerOrientation(const glm::vec3& forward, const glm::vec3& up) {
    m_listenerForward = forward;
    m_listenerUp = up;
    // TODO: Update OpenAL listener
    // float orientation[] = {forward.x, forward.y, forward.z, up.x, up.y, up.z};
    // alListenerfv(AL_ORIENTATION, orientation);
}

void AudioManager::update(float deltaTime) {
    (void)deltaTime; // TODO: Use when implementing streaming/cleanup
    // Update any time-based audio effects
    // Clean up finished audio sources
    // Update streaming audio if needed
}

unsigned int AudioManager::createSource() {
    // TODO: Create OpenAL source
    // unsigned int sourceId;
    // alGenSources(1, &sourceId);
    // return sourceId;
    return 0;
}

unsigned int AudioManager::loadAudioFile(const std::string& filepath) {
    (void)filepath; // TODO: Use when OpenAL is integrated
    // TODO: Load audio file (WAV/OGG) and create OpenAL buffer
    // This would use a library like libsndfile or stb_vorbis
    return 0;
}

void AudioManager::deleteSource(unsigned int sourceId) {
    (void)sourceId; // TODO: Use when OpenAL is integrated
    // TODO: Delete OpenAL source
    // alDeleteSources(1, &sourceId);
}

void AudioManager::deleteBuffer(unsigned int bufferId) {
    (void)bufferId; // TODO: Use when OpenAL is integrated
    // TODO: Delete OpenAL buffer
    // alDeleteBuffers(1, &bufferId);
}

void AudioManager::initializeEventMap() {
    // Map audio events to sound names
    m_eventMap[AudioEvent::SWORD_SWING] = "sword_swing";
    m_eventMap[AudioEvent::SWORD_HIT] = "sword_hit";
    m_eventMap[AudioEvent::BOW_SHOOT] = "bow_shoot";
    m_eventMap[AudioEvent::ARROW_HIT] = "arrow_hit";
    
    m_eventMap[AudioEvent::FIREBALL_CAST] = "fireball_cast";
    m_eventMap[AudioEvent::FIREBALL_IMPACT] = "fireball_impact";
    m_eventMap[AudioEvent::HEAL_CAST] = "heal_cast";
    m_eventMap[AudioEvent::LIGHTNING_CAST] = "lightning_cast";
    
    m_eventMap[AudioEvent::ZOMBIE_IDLE] = "zombie_idle";
    m_eventMap[AudioEvent::ZOMBIE_ATTACK] = "zombie_attack";
    m_eventMap[AudioEvent::ZOMBIE_DEATH] = "zombie_death";
    m_eventMap[AudioEvent::SKELETON_IDLE] = "skeleton_idle";
    m_eventMap[AudioEvent::SKELETON_DEATH] = "skeleton_death";
    
    m_eventMap[AudioEvent::PLAYER_HURT] = "player_hurt";
    m_eventMap[AudioEvent::PLAYER_DEATH] = "player_death";
    m_eventMap[AudioEvent::PLAYER_LEVEL_UP] = "player_level_up";
    m_eventMap[AudioEvent::FOOTSTEP] = "footstep";
    m_eventMap[AudioEvent::JUMP] = "jump";
    m_eventMap[AudioEvent::LAND] = "land";
    
    m_eventMap[AudioEvent::DIALOG_OPEN] = "dialog_open";
    m_eventMap[AudioEvent::DIALOG_CLOSE] = "dialog_close";
    m_eventMap[AudioEvent::QUEST_ACCEPT] = "quest_accept";
    m_eventMap[AudioEvent::QUEST_COMPLETE] = "quest_complete";
}

float AudioManager::getCategoryVolume(AudioCategory category) const {
    auto it = m_categoryVolumes.find(category);
    if (it != m_categoryVolumes.end()) {
        return it->second;
    }
    return 1.0f;
}

} // namespace audio
} // namespace clonemine
