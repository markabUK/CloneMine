#pragma once
#include <string>

class ICharacterService {
public:
    virtual ~ICharacterService() = default;
    
    virtual std::string CreateCharacter(const std::string& name, const std::string& characterClass) = 0;
    virtual std::string GetCharacter(const std::string& id) = 0;
    virtual std::string UpdateCharacter(const std::string& id, int level, long long xp, int health, int mana) = 0;
    virtual std::string DeleteCharacter(const std::string& id) = 0;
    virtual std::string ListCharacters() = 0;
};
