#pragma once

#include <string>
#include <vector>
#include <fstream>
#include <glm/glm.hpp>

namespace clonemine {

struct PlayerSaveData {
    glm::vec3 position;
    std::string className;
    int level;
    float health;
    float maxHealth;
    float resource;  // Mana/Energy
    float maxResource;
    int experience;
    
    // Stats
    int strength;
    int dexterity;
    int constitution;
    int intelligence;
    int wisdom;
    int charisma;
    
    void serialize(std::ofstream& file) const {
        file.write(reinterpret_cast<const char*>(&position), sizeof(position));
        
        size_t classNameLen = className.size();
        file.write(reinterpret_cast<const char*>(&classNameLen), sizeof(classNameLen));
        file.write(className.c_str(), classNameLen);
        
        file.write(reinterpret_cast<const char*>(&level), sizeof(level));
        file.write(reinterpret_cast<const char*>(&health), sizeof(health));
        file.write(reinterpret_cast<const char*>(&maxHealth), sizeof(maxHealth));
        file.write(reinterpret_cast<const char*>(&resource), sizeof(resource));
        file.write(reinterpret_cast<const char*>(&maxResource), sizeof(maxResource));
        file.write(reinterpret_cast<const char*>(&experience), sizeof(experience));
        
        file.write(reinterpret_cast<const char*>(&strength), sizeof(strength));
        file.write(reinterpret_cast<const char*>(&dexterity), sizeof(dexterity));
        file.write(reinterpret_cast<const char*>(&constitution), sizeof(constitution));
        file.write(reinterpret_cast<const char*>(&intelligence), sizeof(intelligence));
        file.write(reinterpret_cast<const char*>(&wisdom), sizeof(wisdom));
        file.write(reinterpret_cast<const char*>(&charisma), sizeof(charisma));
    }
    
    void deserialize(std::ifstream& file) {
        file.read(reinterpret_cast<char*>(&position), sizeof(position));
        
        size_t classNameLen;
        file.read(reinterpret_cast<char*>(&classNameLen), sizeof(classNameLen));
        className.resize(classNameLen);
        file.read(&className[0], classNameLen);
        
        file.read(reinterpret_cast<char*>(&level), sizeof(level));
        file.read(reinterpret_cast<char*>(&health), sizeof(health));
        file.read(reinterpret_cast<char*>(&maxHealth), sizeof(maxHealth));
        file.read(reinterpret_cast<char*>(&resource), sizeof(resource));
        file.read(reinterpret_cast<char*>(&maxResource), sizeof(maxResource));
        file.read(reinterpret_cast<char*>(&experience), sizeof(experience));
        
        file.read(reinterpret_cast<char*>(&strength), sizeof(strength));
        file.read(reinterpret_cast<char*>(&dexterity), sizeof(dexterity));
        file.read(reinterpret_cast<char*>(&constitution), sizeof(constitution));
        file.read(reinterpret_cast<char*>(&intelligence), sizeof(intelligence));
        file.read(reinterpret_cast<char*>(&wisdom), sizeof(wisdom));
        file.read(reinterpret_cast<char*>(&charisma), sizeof(charisma));
    }
};

struct ItemData {
    std::string itemId;
    int quantity;
    float durability;
    
    void serialize(std::ofstream& file) const {
        size_t idLen = itemId.size();
        file.write(reinterpret_cast<const char*>(&idLen), sizeof(idLen));
        file.write(itemId.c_str(), idLen);
        file.write(reinterpret_cast<const char*>(&quantity), sizeof(quantity));
        file.write(reinterpret_cast<const char*>(&durability), sizeof(durability));
    }
    
    void deserialize(std::ifstream& file) {
        size_t idLen;
        file.read(reinterpret_cast<char*>(&idLen), sizeof(idLen));
        itemId.resize(idLen);
        file.read(&itemId[0], idLen);
        file.read(reinterpret_cast<char*>(&quantity), sizeof(quantity));
        file.read(reinterpret_cast<char*>(&durability), sizeof(durability));
    }
};

class SaveSystem {
public:
    static bool saveGame(const std::string& saveSlot, const PlayerSaveData& playerData,
                        const std::vector<ItemData>& inventory) {
        std::string filename = "saves/" + saveSlot + "/player.dat";
        std::ofstream file(filename, std::ios::binary);
        
        if (!file.is_open()) {
            return false;
        }
        
        // Write header
        uint32_t version = 1;
        file.write(reinterpret_cast<const char*>(&version), sizeof(version));
        
        // Write player data
        playerData.serialize(file);
        
        // Write inventory
        size_t inventorySize = inventory.size();
        file.write(reinterpret_cast<const char*>(&inventorySize), sizeof(inventorySize));
        for (const auto& item : inventory) {
            item.serialize(file);
        }
        
        file.close();
        return true;
    }
    
    static bool loadGame(const std::string& saveSlot, PlayerSaveData& playerData,
                        std::vector<ItemData>& inventory) {
        std::string filename = "saves/" + saveSlot + "/player.dat";
        std::ifstream file(filename, std::ios::binary);
        
        if (!file.is_open()) {
            return false;
        }
        
        // Read header
        uint32_t version;
        file.read(reinterpret_cast<char*>(&version), sizeof(version));
        
        if (version != 1) {
            return false;  // Unsupported version
        }
        
        // Read player data
        playerData.deserialize(file);
        
        // Read inventory
        size_t inventorySize;
        file.read(reinterpret_cast<char*>(&inventorySize), sizeof(inventorySize));
        inventory.resize(inventorySize);
        for (auto& item : inventory) {
            item.deserialize(file);
        }
        
        file.close();
        return true;
    }
    
    static std::vector<std::string> getAvailableSaves() {
        std::vector<std::string> saves;
        // Scan saves directory for slots
        // Implementation would use filesystem library
        return saves;
    }
};

} // namespace clonemine
