#include "InteractionSystem.h"
#include "World.h"
#include "Player.h"
#include "Chunk.h"
#include <cmath>

namespace clonemine {

InteractionSystem::InteractionSystem(World* world, Player* player)
    : m_world(world), m_player(player) {
}

void InteractionSystem::update(float deltaTime) {
    // Update cooldown
    if (m_interactionCooldown > 0.0f) {
        m_interactionCooldown -= deltaTime;
    }
    
    // Get player position and look direction
    glm::vec3 playerPos = m_player->getPosition();
    glm::vec3 lookDir = m_player->getLookDirection();
    
    // Perform raycast to find target
    m_currentTarget = raycast(playerPos, lookDir, MAX_INTERACTION_DISTANCE);
    
    // Also check for nearby item pickups
    if (!m_currentTarget.hit) {
        checkItemPickups(playerPos, ITEM_PICKUP_RADIUS, m_currentTarget);
    }
}

bool InteractionSystem::interact() {
    if (!canInteract() || m_interactionCooldown > 0.0f) {
        return false;
    }
    
    m_interactionCooldown = INTERACTION_DELAY;
    
    switch (m_currentTarget.type) {
        case InteractionType::BLOCK:
            return breakBlock();
            
        case InteractionType::ITEM_PICKUP:
            pickupNearbyItems(ITEM_PICKUP_RADIUS);
            return true;
            
        case InteractionType::CHEST:
            // Open chest UI (implemented in UI system)
            return true;
            
        case InteractionType::NPC:
            // Start dialogue (implemented in dialogue system)
            return true;
            
        case InteractionType::BED:
            // Use bed (implemented in rest system)
            return true;
            
        case InteractionType::CAMPFIRE:
            // Use campfire (implemented in rest system)
            return true;
            
        default:
            return false;
    }
}

void InteractionSystem::pickupNearbyItems(float radius) {
    // TODO: Implement item entity system
    // For now, this is a placeholder
    // Real implementation would iterate through item entities in world
    // and add them to player inventory if within radius
}

bool InteractionSystem::breakBlock() {
    if (m_currentTarget.type != InteractionType::BLOCK) {
        return false;
    }
    
    glm::ivec3 blockPos = glm::ivec3(m_currentTarget.position);
    
    // Get block type
    uint8_t blockType = m_world->getBlock(blockPos.x, blockPos.y, blockPos.z);
    
    if (blockType == 0) {
        return false; // Air block
    }
    
    // Set block to air
    m_world->setBlock(blockPos.x, blockPos.y, blockPos.z, 0);
    
    // TODO: Drop item entity for the broken block
    // TODO: Play break sound/particles
    
    return true;
}

bool InteractionSystem::placeBlock(uint8_t blockType) {
    if (m_currentTarget.type != InteractionType::BLOCK || blockType == 0) {
        return false;
    }
    
    // Place block adjacent to hit block, using hit normal
    glm::ivec3 placePos = glm::ivec3(m_currentTarget.position) + glm::ivec3(m_currentTarget.normal);
    
    // Check if position is valid (not occupied, not inside player)
    glm::vec3 playerPos = m_player->getPosition();
    glm::vec3 placeWorldPos = glm::vec3(placePos);
    
    float distToPlayer = glm::distance(playerPos, placeWorldPos);
    if (distToPlayer < 1.0f) {
        return false; // Too close to player
    }
    
    // Set block
    m_world->setBlock(placePos.x, placePos.y, placePos.z, blockType);
    
    // TODO: Play place sound
    
    return true;
}

RayHitResult InteractionSystem::raycast(const glm::vec3& origin, const glm::vec3& direction, float maxDistance) {
    RayHitResult result;
    
    // First check blocks (most common interaction)
    if (raycastBlocks(origin, direction, maxDistance, result)) {
        return result;
    }
    
    // Then check NPCs
    if (checkNPCs(origin, direction, maxDistance, result)) {
        return result;
    }
    
    // Then check chests/objects
    if (checkChests(origin, direction, maxDistance, result)) {
        return result;
    }
    
    return result; // No hit
}

bool InteractionSystem::raycastBlocks(const glm::vec3& origin, const glm::vec3& direction, 
                                      float maxDistance, RayHitResult& result) {
    // DDA (Digital Differential Analyzer) algorithm for voxel raycast
    glm::vec3 pos = origin;
    glm::ivec3 blockPos = glm::ivec3(std::floor(pos.x), std::floor(pos.y), std::floor(pos.z));
    
    glm::vec3 step = glm::sign(direction);
    glm::vec3 tDelta = glm::abs(1.0f / direction);
    glm::vec3 dist = (step * (glm::vec3(blockPos) - pos) + (step * 0.5f) + 0.5f) * tDelta;
    
    float totalDistance = 0.0f;
    
    while (totalDistance < maxDistance) {
        // Check current block
        uint8_t blockType = m_world->getBlock(blockPos.x, blockPos.y, blockPos.z);
        
        if (blockType != 0) { // Not air
            result.hit = true;
            result.position = glm::vec3(blockPos);
            result.distance = totalDistance;
            result.type = InteractionType::BLOCK;
            result.target = nullptr; // Block is identified by position
            
            // Calculate normal based on which face was hit
            if (dist.x < dist.y && dist.x < dist.z) {
                result.normal = glm::vec3(-step.x, 0.0f, 0.0f);
            } else if (dist.y < dist.z) {
                result.normal = glm::vec3(0.0f, -step.y, 0.0f);
            } else {
                result.normal = glm::vec3(0.0f, 0.0f, -step.z);
            }
            
            return true;
        }
        
        // Step to next block
        if (dist.x < dist.y && dist.x < dist.z) {
            totalDistance = dist.x;
            dist.x += tDelta.x;
            blockPos.x += static_cast<int>(step.x);
        } else if (dist.y < dist.z) {
            totalDistance = dist.y;
            dist.y += tDelta.y;
            blockPos.y += static_cast<int>(step.y);
        } else {
            totalDistance = dist.z;
            dist.z += tDelta.z;
            blockPos.z += static_cast<int>(step.z);
        }
    }
    
    return false;
}

bool InteractionSystem::checkItemPickups(const glm::vec3& playerPos, float radius, RayHitResult& result) {
    // TODO: Implement when item entity system is added
    // Would iterate through nearby item entities and find closest one within radius
    return false;
}

bool InteractionSystem::checkNPCs(const glm::vec3& playerPos, const glm::vec3& lookDir, 
                                  float maxDistance, RayHitResult& result) {
    // TODO: Implement when NPC system is added
    // Would check for NPCs within cone in front of player
    return false;
}

bool InteractionSystem::checkChests(const glm::vec3& playerPos, const glm::vec3& lookDir, 
                                    float maxDistance, RayHitResult& result) {
    // TODO: Implement when chest system is added
    // Would check for chests at block positions
    return false;
}

} // namespace clonemine
