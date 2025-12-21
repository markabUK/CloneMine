#pragma once
#include "../Interfaces/ICharacterService.h"
#include "../Interfaces/ICharacterRepository.h"
#include "../Interfaces/IInputValidator.h"
#include <memory>
#include <sstream>
#include <random>
#include <iomanip>

class CharacterService : public ICharacterService {
private:
    std::shared_ptr<ICharacterRepository> repository;
    std::shared_ptr<IInputValidator> validator;
    
    std::string GenerateCharacterId() {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(0, 15);
        
        const char* hex = "0123456789abcdef";
        std::string id = "char_";
        for (int i = 0; i < 12; ++i) {
            id += hex[dis(gen)];
        }
        return id;
    }
    
public:
    CharacterService(std::shared_ptr<ICharacterRepository> repo,
                    std::shared_ptr<IInputValidator> val)
        : repository(repo), validator(val) {}
    
    std::string CreateCharacter(const std::string& name, const std::string& characterClass) override {
        auto [validName, nameError] = validator->ValidateCharacterName(name);
        if (!validName) {
            return "ERROR " + nameError;
        }
        
        auto [validClass, classError] = validator->ValidateCharacterClass(characterClass);
        if (!validClass) {
            return "ERROR " + classError;
        }
        
        std::string id = GenerateCharacterId();
        Character character(id, name, characterClass);
        repository->SaveCharacter(character);
        
        return "SUCCESS Character created with ID: " + id;
    }
    
    std::string GetCharacter(const std::string& id) override {
        auto [valid, error] = validator->ValidateCharacterId(id);
        if (!valid) {
            return "ERROR " + error;
        }
        
        auto character = repository->GetCharacter(id);
        if (!character) {
            return "ERROR Character not found";
        }
        
        std::ostringstream oss;
        oss << "SUCCESS " << character->id << " " << character->name << " "
            << character->level << " " << character->xp << " "
            << character->health << " " << character->mana << " "
            << character->characterClass;
        return oss.str();
    }
    
    std::string UpdateCharacter(const std::string& id, int level, long long xp, 
                               int health, int mana) override {
        auto [validId, idError] = validator->ValidateCharacterId(id);
        if (!validId) {
            return "ERROR " + idError;
        }
        
        auto [validLevel, levelError] = validator->ValidateLevel(level);
        if (!validLevel) {
            return "ERROR " + levelError;
        }
        
        auto [validXP, xpError] = validator->ValidateXP(xp);
        if (!validXP) {
            return "ERROR " + xpError;
        }
        
        auto [validHealth, healthError] = validator->ValidateHealth(health);
        if (!validHealth) {
            return "ERROR " + healthError;
        }
        
        auto [validMana, manaError] = validator->ValidateMana(mana);
        if (!validMana) {
            return "ERROR " + manaError;
        }
        
        auto character = repository->GetCharacter(id);
        if (!character) {
            return "ERROR Character not found";
        }
        
        character->level = level;
        character->xp = xp;
        character->health = health;
        character->mana = mana;
        repository->SaveCharacter(*character);
        
        return "SUCCESS Character updated";
    }
    
    std::string DeleteCharacter(const std::string& id) override {
        auto [valid, error] = validator->ValidateCharacterId(id);
        if (!valid) {
            return "ERROR " + error;
        }
        
        bool deleted = repository->DeleteCharacter(id);
        if (!deleted) {
            return "ERROR Character not found";
        }
        
        return "SUCCESS Character deleted";
    }
    
    std::string ListCharacters() override {
        auto characters = repository->GetAllCharacters();
        
        std::ostringstream oss;
        oss << "CHARACTERS " << characters.size();
        for (const auto& character : characters) {
            oss << "\n" << character.id << " " << character.name << " "
                << character.level << " " << character.xp << " "
                << character.health << " " << character.mana << " "
                << character.characterClass;
        }
        return oss.str();
    }
};
