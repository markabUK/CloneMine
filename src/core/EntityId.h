#pragma once

#include <string>
#include <functional>

namespace clonemine {

/**
 * Entity types in the game world.
 * Used to distinguish between entities with the same name
 * (e.g., a player named "Lich" vs a monster named "Lich").
 */
enum class EntityType {
    PLAYER,     // Player characters
    MONSTER,    // Enemy creatures
    NPC,        // Non-player characters (friendly)
    OBJECT,     // Interactable objects
    PROJECTILE, // Spells, arrows, etc.
    UNKNOWN
};

/**
 * Qualified entity identifier that includes both name and type.
 * This ensures unique identification even when a player and monster
 * share the same name.
 */
struct EntityId {
    std::string name;
    EntityType type;
    
    // Default constructor
    EntityId() : name(""), type(EntityType::UNKNOWN) {}
    
    // Constructor with name and type
    EntityId(const std::string& entityName, EntityType entityType)
        : name(entityName), type(entityType) {}
    
    // Constructor from string with type prefix (e.g., "player:Arthas", "monster:Lich")
    explicit EntityId(const std::string& qualifiedId) {
        parseQualifiedId(qualifiedId);
    }
    
    // Create a qualified string representation "type:name"
    [[nodiscard]] std::string toQualifiedString() const {
        return getTypePrefix() + ":" + name;
    }
    
    // For backward compatibility - returns just the name
    [[nodiscard]] const std::string& getName() const { return name; }
    
    // Get the entity type
    [[nodiscard]] EntityType getType() const { return type; }
    
    // Check if this is a player entity
    [[nodiscard]] bool isPlayer() const { return type == EntityType::PLAYER; }
    
    // Check if this is a monster entity
    [[nodiscard]] bool isMonster() const { return type == EntityType::MONSTER; }
    
    // Check if this is an NPC entity
    [[nodiscard]] bool isNPC() const { return type == EntityType::NPC; }
    
    // Check if valid (has a name and known type)
    [[nodiscard]] bool isValid() const {
        return !name.empty() && type != EntityType::UNKNOWN;
    }
    
    // Equality comparison (both name and type must match)
    bool operator==(const EntityId& other) const {
        return name == other.name && type == other.type;
    }
    
    bool operator!=(const EntityId& other) const {
        return !(*this == other);
    }
    
    // Less-than for map/set ordering
    bool operator<(const EntityId& other) const {
        if (type != other.type) return static_cast<int>(type) < static_cast<int>(other.type);
        return name < other.name;
    }
    
private:
    [[nodiscard]] std::string getTypePrefix() const {
        switch (type) {
            case EntityType::PLAYER: return "player";
            case EntityType::MONSTER: return "monster";
            case EntityType::NPC: return "npc";
            case EntityType::OBJECT: return "object";
            case EntityType::PROJECTILE: return "projectile";
            default: return "unknown";
        }
    }
    
    void parseQualifiedId(const std::string& qualifiedId) {
        auto colonPos = qualifiedId.find(':');
        if (colonPos != std::string::npos) {
            std::string typeStr = qualifiedId.substr(0, colonPos);
            name = qualifiedId.substr(colonPos + 1);
            
            if (typeStr == "player") type = EntityType::PLAYER;
            else if (typeStr == "monster") type = EntityType::MONSTER;
            else if (typeStr == "npc") type = EntityType::NPC;
            else if (typeStr == "object") type = EntityType::OBJECT;
            else if (typeStr == "projectile") type = EntityType::PROJECTILE;
            else type = EntityType::UNKNOWN;
        } else {
            // No prefix - treat as unknown type with full string as name
            // This allows backward compatibility with simple string IDs
            name = qualifiedId;
            type = EntityType::UNKNOWN;
        }
    }
};

// Helper functions for creating EntityId objects
inline EntityId makePlayerId(const std::string& name) {
    return EntityId(name, EntityType::PLAYER);
}

inline EntityId makeMonsterId(const std::string& name) {
    return EntityId(name, EntityType::MONSTER);
}

inline EntityId makeNPCId(const std::string& name) {
    return EntityId(name, EntityType::NPC);
}

inline EntityId makeObjectId(const std::string& name) {
    return EntityId(name, EntityType::OBJECT);
}

// Convert entity type to string for display/logging
inline std::string entityTypeToString(EntityType type) {
    switch (type) {
        case EntityType::PLAYER: return "Player";
        case EntityType::MONSTER: return "Monster";
        case EntityType::NPC: return "NPC";
        case EntityType::OBJECT: return "Object";
        case EntityType::PROJECTILE: return "Projectile";
        default: return "Unknown";
    }
}

// Parse entity type from string
inline EntityType stringToEntityType(const std::string& str) {
    if (str == "player" || str == "Player") return EntityType::PLAYER;
    if (str == "monster" || str == "Monster") return EntityType::MONSTER;
    if (str == "npc" || str == "NPC") return EntityType::NPC;
    if (str == "object" || str == "Object") return EntityType::OBJECT;
    if (str == "projectile" || str == "Projectile") return EntityType::PROJECTILE;
    return EntityType::UNKNOWN;
}

} // namespace clonemine

// Hash function for EntityId (allows use in unordered containers)
namespace std {
template<>
struct hash<clonemine::EntityId> {
    size_t operator()(const clonemine::EntityId& id) const {
        size_t h1 = std::hash<std::string>{}(id.name);
        size_t h2 = std::hash<int>{}(static_cast<int>(id.type));
        return h1 ^ (h2 << 1);
    }
};
}
