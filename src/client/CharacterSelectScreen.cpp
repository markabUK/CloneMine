#include "CharacterSelectScreen.h"
#include <iostream>
#include <chrono>

namespace clonemine {
namespace client {

CharacterSelectScreen::CharacterSelectScreen() {
    reset();
}

void CharacterSelectScreen::reset() {
    m_characters.clear();
    m_selectedIndex = 0;
    m_createMode = false;
    m_newCharacterName.clear();
    recordActivity();
}

void CharacterSelectScreen::recordActivity() {
    auto now = std::chrono::steady_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::seconds>(now.time_since_epoch());
    m_lastActivityTime = static_cast<float>(duration.count());
}

void CharacterSelectScreen::processInput() {
    // TODO: Integrate with actual input system
    // For now, this is a stub implementation
    // Real implementation would:
    // - Handle UP/DOWN arrow keys to change selection
    // - Handle ENTER to select character
    // - Handle 'C' key to create new character
    // - Handle text input for character name during creation
    // - Call recordActivity() on any input
}

void CharacterSelectScreen::update(float deltaTime) {
    // Character select screen update logic
    // Timeout is handled by ClientApplication
}

void CharacterSelectScreen::render() {
    // TODO: Integrate with actual rendering system
    // This is a stub for the character select screen rendering
    // Real implementation would:
    // - Draw title "Select Your Character"
    // - Draw list of characters with details (name, level, class)
    // - Highlight selected character
    // - Draw "Create New Character" option
    // - Draw character details for selected character
    
    std::cout << "=== CHARACTER SELECT ===" << std::endl;
    
    if (m_createMode) {
        std::cout << "Creating new character..." << std::endl;
        std::cout << "Name: " << m_newCharacterName << "_" << std::endl;
        std::cout << "Press ENTER to confirm, ESC to cancel" << std::endl;
    } else {
        std::cout << "Available characters:" << std::endl;
        
        for (size_t i = 0; i < m_characters.size(); ++i) {
            const auto& character = m_characters[i];
            std::cout << (i == static_cast<size_t>(m_selectedIndex) ? "> " : "  ");
            std::cout << character.name << " - Level " << character.level 
                      << " " << character.characterClass << std::endl;
        }
        
        if (m_characters.size() < 5) {
            std::cout << (m_selectedIndex == static_cast<int>(m_characters.size()) ? "> " : "  ");
            std::cout << "[Create New Character]" << std::endl;
        }
        
        std::cout << "\nUse UP/DOWN to select, ENTER to confirm, C to create" << std::endl;
    }
    
    std::cout << "========================" << std::endl;
}

void CharacterSelectScreen::setCharacters(const std::vector<character::CharacterData>& characters) {
    m_characters = characters;
    m_selectedIndex = 0;
    recordActivity();
}

void CharacterSelectScreen::selectCharacter() {
    if (m_selectedIndex >= 0 && m_selectedIndex < static_cast<int>(m_characters.size())) {
        if (m_selectCallback) {
            m_selectCallback(m_selectedIndex);
        }
    } else if (m_selectedIndex == static_cast<int>(m_characters.size())) {
        // Selected "Create New Character"
        startCharacterCreation();
    }
    recordActivity();
}

void CharacterSelectScreen::startCharacterCreation() {
    if (m_characters.size() >= 5) {
        std::cout << "Maximum number of characters reached (5)" << std::endl;
        return;
    }
    
    m_createMode = true;
    m_newCharacterName.clear();
    recordActivity();
}

void CharacterSelectScreen::confirmCharacterCreation() {
    if (m_newCharacterName.empty()) {
        std::cout << "Character name cannot be empty" << std::endl;
        return;
    }
    
    if (m_createCallback) {
        m_createCallback(m_newCharacterName);
    }
    
    m_createMode = false;
    m_newCharacterName.clear();
    recordActivity();
}

} // namespace client
} // namespace clonemine
