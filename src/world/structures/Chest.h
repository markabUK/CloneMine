#pragma once

#include <vector>
#include <glm/glm.hpp>
#include "../Block.h"

namespace clonemine {

enum class ChestType {
    REGULAR,    // 27 slots
    LARGE,      // 54 slots (double chest)
    LOCKED      // Requires key
};

struct ItemStack {
    int itemId{0};
    int count{0};
    int durability{100};
};

class Chest {
public:
    Chest(const glm::vec3& position, ChestType type = ChestType::REGULAR);
    
    // Item management
    bool addItem(const ItemStack& item);
    ItemStack removeItem(int slot);
    ItemStack getItem(int slot) const;
    bool setItem(int slot, const ItemStack& item);
    
    // Properties
    int getSlotCount() const;
    bool isLocked() const { return m_type == ChestType::LOCKED; }
    bool unlock(int keyId);
    
    // Position
    glm::vec3 getPosition() const { return m_position; }
    
    // Generate loot (for dungeon/world chests)
    void generateLoot(int level, const std::string& lootTable);
    
    // Serialization for save/load
    void serialize(std::vector<uint8_t>& data) const;
    void deserialize(const std::vector<uint8_t>& data, size_t& offset);
    
private:
    glm::vec3 m_position;
    ChestType m_type;
    std::vector<ItemStack> m_inventory;
    bool m_locked{false};
    int m_requiredKeyId{0};
    
    int getMaxSlots() const;
};

} // namespace clonemine
