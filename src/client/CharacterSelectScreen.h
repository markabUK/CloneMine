#pragma once

#include "../character/CharacterData.h"
#include <string>
#include <vector>
#include <functional>
#include <memory>

namespace clonemine {
namespace client {

class CharacterSelectScreen {
public:
    CharacterSelectScreen();
    ~CharacterSelectScreen() = default;

    // Process input for character selection
    void processInput();
    
    // Update character select screen state
    void update(float deltaTime);
    
    // Render the character select screen
    void render();
    
    // Set the list of available characters
    void setCharacters(const std::vector<character::CharacterData>& characters);
    
    // Set callback for character selection
    void setSelectCallback(std::function<void(int)> callback) {
        m_selectCallback = callback;
    }
    
    // Set callback for character creation
    void setCreateCallback(std::function<void(const std::string&)> callback) {
        m_createCallback = callback;
    }
    
    // Get selected character index
    [[nodiscard]] int getSelectedIndex() const { return m_selectedIndex; }
    
    // Reset the screen
    void reset();
    
    // Get last activity time (for timeout detection)
    [[nodiscard]] float getLastActivityTime() const { return m_lastActivityTime; }
    
    // Record activity (resets timeout)
    void recordActivity();

private:
    std::vector<character::CharacterData> m_characters;
    int m_selectedIndex{0};
    bool m_createMode{false};
    std::string m_newCharacterName;
    float m_lastActivityTime{0.0f};
    
    std::function<void(int)> m_selectCallback;
    std::function<void(const std::string&)> m_createCallback;
    
    void selectCharacter();
    void startCharacterCreation();
    void confirmCharacterCreation();
};

} // namespace client
} // namespace clonemine
