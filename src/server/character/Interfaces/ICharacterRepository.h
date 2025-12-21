#pragma once
#include <string>
#include <vector>
#include <optional>
#include "../Models/Character.h"

class ICharacterRepository {
public:
    virtual ~ICharacterRepository() = default;
    
    virtual void SaveCharacter(const Character& character) = 0;
    virtual std::optional<Character> GetCharacter(const std::string& id) = 0;
    virtual bool DeleteCharacter(const std::string& id) = 0;
    virtual std::vector<Character> GetAllCharacters() = 0;
};
