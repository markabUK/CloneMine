#include "Chest.h"
#include <algorithm>
#include <cstring>

namespace clonemine {

Chest::Chest(const glm::vec3& position, ChestType type)
    : m_position(position), m_type(type) {
    m_inventory.resize(getMaxSlots());
    if (type == ChestType::LOCKED) {
        m_locked = true;
        m_requiredKeyId = 1000 + (rand() % 100);
    }
}

bool Chest::addItem(const ItemStack& item) {
    // Try to stack with existing items
    for (auto& slot : m_inventory) {
        if (slot.itemId == item.itemId && slot.count < 64) {
            int spaceLeft = 64 - slot.count;
            int toAdd = std::min(spaceLeft, item.count);
            slot.count += toAdd;
            if (toAdd == item.count) return true;
        }
    }
    
    // Find empty slot
    for (auto& slot : m_inventory) {
        if (slot.itemId == 0) {
            slot = item;
            return true;
        }
    }
    
    return false;
}

ItemStack Chest::removeItem(int slot) {
    if (slot < 0 || slot >= static_cast<int>(m_inventory.size())) {
        return ItemStack{};
    }
    
    ItemStack item = m_inventory[slot];
    m_inventory[slot] = ItemStack{};
    return item;
}

ItemStack Chest::getItem(int slot) const {
    if (slot < 0 || slot >= static_cast<int>(m_inventory.size())) {
        return ItemStack{};
    }
    return m_inventory[slot];
}

bool Chest::setItem(int slot, const ItemStack& item) {
    if (slot < 0 || slot >= static_cast<int>(m_inventory.size())) {
        return false;
    }
    m_inventory[slot] = item;
    return true;
}

int Chest::getSlotCount() const {
    return getMaxSlots();
}

bool Chest::unlock(int keyId) {
    if (m_type != ChestType::LOCKED) return true;
    if (keyId == m_requiredKeyId) {
        m_locked = false;
        return true;
    }
    return false;
}

void Chest::generateLoot(int level, const std::string& lootTable) {
    int numItems = 3 + (rand() % 5);
    
    for (int i = 0; i < numItems && i < getMaxSlots(); ++i) {
        ItemStack item;
        item.itemId = 100 + (rand() % 200);
        item.count = 1 + (rand() % 5);
        item.durability = 80 + (rand() % 20);
        m_inventory[i] = item;
    }
}

void Chest::serialize(std::vector<uint8_t>& data) const {
    data.insert(data.end(), reinterpret_cast<const uint8_t*>(&m_position), 
                reinterpret_cast<const uint8_t*>(&m_position) + sizeof(glm::vec3));
    
    data.push_back(static_cast<uint8_t>(m_type));
    data.push_back(m_locked ? 1 : 0);
    
    for (const auto& item : m_inventory) {
        data.insert(data.end(), reinterpret_cast<const uint8_t*>(&item),
                    reinterpret_cast<const uint8_t*>(&item) + sizeof(ItemStack));
    }
}

void Chest::deserialize(const std::vector<uint8_t>& data, size_t& offset) {
    std::memcpy(&m_position, &data[offset], sizeof(glm::vec3));
    offset += sizeof(glm::vec3);
    
    m_type = static_cast<ChestType>(data[offset++]);
    m_locked = data[offset++] != 0;
    
    m_inventory.resize(getMaxSlots());
    for (auto& item : m_inventory) {
        std::memcpy(&item, &data[offset], sizeof(ItemStack));
        offset += sizeof(ItemStack);
    }
}

int Chest::getMaxSlots() const {
    switch (m_type) {
        case ChestType::REGULAR: return 27;
        case ChestType::LARGE: return 54;
        case ChestType::LOCKED: return 27;
        default: return 27;
    }
}

} // namespace clonemine
