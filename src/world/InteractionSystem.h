#pragma once

#include <glm/glm.hpp>
#include <string>
#include <vector>
#include <functional>

namespace clonemine {

class Player;
class World;
class Block;
class Chest;
class NPC;

enum class InteractionType {
    NONE,
    BLOCK,          // Block breaking/placing
    ITEM_PICKUP,    // Pick up item from ground
    CHEST,          // Open chest
    NPC,            // Talk to NPC
    BED,            // Use bed
    CAMPFIRE,       // Use campfire
    DOOR            // Open/close door
};

struct RayHitResult {
    bool hit = false;
    glm::vec3 position{0.0f};
    glm::vec3 normal{0.0f};
    float distance = 0.0f;
    InteractionType type = InteractionType::NONE;
    void* target = nullptr; // Pointer to the object hit (Block*, Chest*, NPC*, etc.)
};

class InteractionSystem {
public:
    InteractionSystem(World* world, Player* player);
    
    // Update interaction system (check for nearby interactables)
    void update(float deltaTime);
    
    // Perform interaction with targeted object
    bool interact();
    
    // Get current target
    const RayHitResult& getCurrentTarget() const { return m_currentTarget; }
    
    // Check if player can interact with something
    bool canInteract() const { return m_currentTarget.hit; }
    
    // Item pickup
    void pickupNearbyItems(float radius);
    
    // Block interaction
    bool breakBlock();
    bool placeBlock(uint8_t blockType);
    
    // Ray casting
    RayHitResult raycast(const glm::vec3& origin, const glm::vec3& direction, float maxDistance);
    
private:
    // Helper functions
    bool raycastBlocks(const glm::vec3& origin, const glm::vec3& direction, float maxDistance, RayHitResult& result);
    bool checkItemPickups(const glm::vec3& playerPos, float radius, RayHitResult& result);
    bool checkNPCs(const glm::vec3& playerPos, const glm::vec3& lookDir, float maxDistance, RayHitResult& result);
    bool checkChests(const glm::vec3& playerPos, const glm::vec3& lookDir, float maxDistance, RayHitResult& result);
    
    World* m_world;
    Player* m_player;
    RayHitResult m_currentTarget;
    
    float m_interactionCooldown = 0.0f;
    static constexpr float INTERACTION_DELAY = 0.2f;
    static constexpr float MAX_INTERACTION_DISTANCE = 5.0f;
    static constexpr float ITEM_PICKUP_RADIUS = 2.0f;
};

} // namespace clonemine
