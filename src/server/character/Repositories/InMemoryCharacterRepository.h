#pragma once
#include "../Interfaces/ICharacterRepository.h"
#include <unordered_map>
#include <mutex>

class InMemoryCharacterRepository : public ICharacterRepository {
private:
    std::unordered_map<std::string, Character> characters;
    mutable std::mutex mutex;
    
public:
    void SaveCharacter(const Character& character) override {
        std::lock_guard<std::mutex> lock(mutex);
        characters[character.id] = character;
    }
    
    std::optional<Character> GetCharacter(const std::string& id) override {
        std::lock_guard<std::mutex> lock(mutex);
        auto it = characters.find(id);
        if (it != characters.end()) {
            return it->second;
        }
        return std::nullopt;
    }
    
    bool DeleteCharacter(const std::string& id) override {
        std::lock_guard<std::mutex> lock(mutex);
        return characters.erase(id) > 0;
    }
    
    std::vector<Character> GetAllCharacters() override {
        std::lock_guard<std::mutex> lock(mutex);
        std::vector<Character> result;
        for (const auto& pair : characters) {
            result.push_back(pair.second);
        }
        return result;
    }
};
