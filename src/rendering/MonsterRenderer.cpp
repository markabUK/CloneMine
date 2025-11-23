#include "MonsterRenderer.h"
#include <cmath>

namespace clonemine {

MonsterRenderer::MonsterRenderer(VkDevice device, VkPhysicalDevice physicalDevice)
    : m_device(device), m_physicalDevice(physicalDevice) {
}

MonsterRenderer::~MonsterRenderer() {
    for (auto& [name, model] : m_models) {
        destroyModel(model);
    }
}

void MonsterRenderer::createBipedModel(const std::string& monsterType, MonsterModel& model) {
    model.type = MonsterModelType::BIPED;
    model.vertices.clear();
    model.indices.clear();
    
    // Color varies by monster type
    glm::vec3 color(0.5f, 0.5f, 0.5f);
    if (monsterType == "Zombie") color = glm::vec3(0.4f, 0.6f, 0.4f);
    else if (monsterType == "Skeleton") color = glm::vec3(0.9f, 0.9f, 0.9f);
    else if (monsterType == "Goblin") color = glm::vec3(0.3f, 0.7f, 0.3f);
    
    // Blocky biped model (Minecraft style)
    // Head
    createBlockyHead(model.vertices, model.indices, 
                     glm::vec3(0.0f, 1.5f, 0.0f), 
                     glm::vec3(0.5f, 0.5f, 0.5f), 
                     color);
    
    // Body
    createBlockyBody(model.vertices, model.indices,
                     glm::vec3(0.0f, 0.75f, 0.0f),
                     glm::vec3(0.5f, 0.75f, 0.25f),
                     color);
    
    // Arms (left and right)
    createBlockyLimb(model.vertices, model.indices,
                     glm::vec3(-0.4f, 1.0f, 0.0f),
                     glm::vec3(0.2f, 0.6f, 0.2f),
                     color);
    
    createBlockyLimb(model.vertices, model.indices,
                     glm::vec3(0.4f, 1.0f, 0.0f),
                     glm::vec3(0.2f, 0.6f, 0.2f),
                     color);
    
    // Legs (left and right)
    createBlockyLimb(model.vertices, model.indices,
                     glm::vec3(-0.15f, 0.25f, 0.0f),
                     glm::vec3(0.2f, 0.5f, 0.2f),
                     color);
    
    createBlockyLimb(model.vertices, model.indices,
                     glm::vec3(0.15f, 0.25f, 0.0f),
                     glm::vec3(0.2f, 0.5f, 0.2f),
                     color);
    
    createVulkanBuffers(model);
}

void MonsterRenderer::createQuadrupedModel(const std::string& monsterType, MonsterModel& model) {
    model.type = MonsterModelType::QUADRUPED;
    model.vertices.clear();
    model.indices.clear();
    
    // Color varies by monster type
    glm::vec3 color(0.6f, 0.4f, 0.2f);
    if (monsterType == "Wolf") color = glm::vec3(0.5f, 0.5f, 0.5f);
    else if (monsterType == "Bear") color = glm::vec3(0.4f, 0.3f, 0.2f);
    else if (monsterType == "Lion") color = glm::vec3(0.8f, 0.6f, 0.3f);
    
    // Blocky quadruped model
    // Head
    createBlockyHead(model.vertices, model.indices,
                     glm::vec3(0.6f, 0.6f, 0.0f),
                     glm::vec3(0.4f, 0.4f, 0.4f),
                     color);
    
    // Body (horizontal)
    createBlockyBody(model.vertices, model.indices,
                     glm::vec3(0.0f, 0.6f, 0.0f),
                     glm::vec3(0.8f, 0.5f, 0.4f),
                     color);
    
    // Four legs
    for (int i = 0; i < 4; ++i) {
        float x = (i % 2 == 0) ? -0.3f : 0.3f;
        float z = (i < 2) ? 0.3f : -0.3f;
        
        createBlockyLimb(model.vertices, model.indices,
                        glm::vec3(x, 0.25f, z),
                        glm::vec3(0.15f, 0.5f, 0.15f),
                        color);
    }
    
    createVulkanBuffers(model);
}

void MonsterRenderer::createFlyingModel(const std::string& monsterType, MonsterModel& model) {
    model.type = MonsterModelType::FLYING;
    model.vertices.clear();
    model.indices.clear();
    
    // Color varies by monster type
    glm::vec3 color(0.6f, 0.2f, 0.2f);
    if (monsterType == "Dragon") color = glm::vec3(0.8f, 0.1f, 0.1f);
    else if (monsterType == "Wyvern") color = glm::vec3(0.5f, 0.3f, 0.6f);
    
    // Dragon-like body
    // Head (elongated)
    createBlockyHead(model.vertices, model.indices,
                     glm::vec3(1.0f, 0.8f, 0.0f),
                     glm::vec3(0.5f, 0.4f, 0.4f),
                     color);
    
    // Body (large)
    createBlockyBody(model.vertices, model.indices,
                     glm::vec3(0.0f, 0.8f, 0.0f),
                     glm::vec3(1.2f, 0.8f, 0.6f),
                     color);
    
    // Wings (large flat panels)
    // Left wing
    addCube(model.vertices, model.indices,
            glm::vec3(-1.0f, 1.0f, 0.0f),
            glm::vec3(1.0f, 0.05f, 0.8f),
            color * 0.9f);
    
    // Right wing
    addCube(model.vertices, model.indices,
            glm::vec3(1.0f, 1.0f, 0.0f),
            glm::vec3(1.0f, 0.05f, 0.8f),
            color * 0.9f);
    
    // Tail
    createBlockyLimb(model.vertices, model.indices,
                     glm::vec3(-0.8f, 0.6f, 0.0f),
                     glm::vec3(0.6f, 0.2f, 0.2f),
                     color);
    
    createVulkanBuffers(model);
}

void MonsterRenderer::createSpecialModel(const std::string& monsterType, MonsterModel& model) {
    model.type = MonsterModelType::SPECIAL;
    model.vertices.clear();
    model.indices.clear();
    
    if (monsterType == "Slime") {
        // Bouncy cube
        glm::vec3 color(0.2f, 0.8f, 0.2f);
        addCube(model.vertices, model.indices,
                glm::vec3(0.0f, 0.4f, 0.0f),
                glm::vec3(0.8f, 0.8f, 0.8f),
                color);
    }
    else if (monsterType == "Spider") {
        // Body + 8 legs
        glm::vec3 color(0.2f, 0.2f, 0.2f);
        
        // Body
        addCube(model.vertices, model.indices,
                glm::vec3(0.0f, 0.3f, 0.0f),
                glm::vec3(0.6f, 0.3f, 0.8f),
                color);
        
        // 8 legs
        for (int i = 0; i < 8; ++i) {
            float angle = (i / 8.0f) * 2.0f * 3.14159f;
            float x = std::cos(angle) * 0.5f;
            float z = std::sin(angle) * 0.5f;
            
            createBlockyLimb(model.vertices, model.indices,
                            glm::vec3(x, 0.15f, z),
                            glm::vec3(0.1f, 0.3f, 0.1f),
                            color);
        }
    }
    
    createVulkanBuffers(model);
}

void MonsterRenderer::createBlockyHead(std::vector<MonsterVertex>& verts, 
                                       std::vector<uint32_t>& indices,
                                       const glm::vec3& position,
                                       const glm::vec3& size,
                                       const glm::vec3& color) {
    addCube(verts, indices, position, size, color);
}

void MonsterRenderer::createBlockyBody(std::vector<MonsterVertex>& verts,
                                       std::vector<uint32_t>& indices,
                                       const glm::vec3& position,
                                       const glm::vec3& size,
                                       const glm::vec3& color) {
    addCube(verts, indices, position, size, color);
}

void MonsterRenderer::createBlockyLimb(std::vector<MonsterVertex>& verts,
                                       std::vector<uint32_t>& indices,
                                       const glm::vec3& position,
                                       const glm::vec3& size,
                                       const glm::vec3& color) {
    addCube(verts, indices, position, size, color);
}

void MonsterRenderer::addCube(std::vector<MonsterVertex>& verts,
                              std::vector<uint32_t>& indices,
                              const glm::vec3& center,
                              const glm::vec3& size,
                              const glm::vec3& color) {
    uint32_t baseIdx = static_cast<uint32_t>(verts.size());
    
    // 8 vertices of a cube
    glm::vec3 halfSize = size * 0.5f;
    glm::vec3 corners[8] = {
        center + glm::vec3(-halfSize.x, -halfSize.y, -halfSize.z),
        center + glm::vec3( halfSize.x, -halfSize.y, -halfSize.z),
        center + glm::vec3( halfSize.x,  halfSize.y, -halfSize.z),
        center + glm::vec3(-halfSize.x,  halfSize.y, -halfSize.z),
        center + glm::vec3(-halfSize.x, -halfSize.y,  halfSize.z),
        center + glm::vec3( halfSize.x, -halfSize.y,  halfSize.z),
        center + glm::vec3( halfSize.x,  halfSize.y,  halfSize.z),
        center + glm::vec3(-halfSize.x,  halfSize.y,  halfSize.z)
    };
    
    // 6 faces, 4 vertices each
    int faceIndices[6][4] = {
        {0, 1, 2, 3}, // Front
        {4, 5, 6, 7}, // Back
        {0, 4, 7, 3}, // Left
        {1, 5, 6, 2}, // Right
        {3, 2, 6, 7}, // Top
        {0, 1, 5, 4}  // Bottom
    };
    
    glm::vec3 normals[6] = {
        {0, 0, -1}, {0, 0, 1}, {-1, 0, 0},
        {1, 0, 0}, {0, 1, 0}, {0, -1, 0}
    };
    
    for (int face = 0; face < 6; ++face) {
        for (int i = 0; i < 4; ++i) {
            MonsterVertex v;
            v.position = corners[faceIndices[face][i]];
            v.normal = normals[face];
            v.color = color;
            v.texCoord = glm::vec2(i % 2, i / 2); // Simple UV
            verts.push_back(v);
        }
        
        uint32_t faceBase = baseIdx + face * 4;
        // Two triangles per face
        indices.push_back(faceBase);
        indices.push_back(faceBase + 1);
        indices.push_back(faceBase + 2);
        
        indices.push_back(faceBase);
        indices.push_back(faceBase + 2);
        indices.push_back(faceBase + 3);
    }
}

void MonsterRenderer::updateAnimation(AnimationState& anim, float deltaTime) {
    anim.time += deltaTime * anim.speed;
    
    // Reset animation time if not looping
    if (!anim.looping) {
        // Animation durations
        float duration = 1.0f;
        switch (anim.current) {
            case MonsterAnimation::IDLE: duration = 2.0f; break;
            case MonsterAnimation::WALK: duration = 1.0f; break;
            case MonsterAnimation::ATTACK: duration = 0.5f; break;
            case MonsterAnimation::DAMAGE: duration = 0.3f; break;
            case MonsterAnimation::DEATH: duration = 1.5f; break;
            default: break;
        }
        
        if (anim.time > duration) {
            anim.time = duration;
        }
    } else {
        // Loop animation
        if (anim.time > 2.0f) {
            anim.time = 0.0f;
        }
    }
}

void MonsterRenderer::applyAnimation(const MonsterModel& model,
                                     const AnimationState& anim,
                                     std::vector<glm::mat4>& boneTransforms) {
    // Simple procedural animation based on animation type
    (void)model;
    (void)anim;
    (void)boneTransforms;
    
    // In a full implementation, this would apply skeletal animation transforms
}

MonsterModel* MonsterRenderer::getModel(const std::string& monsterType) {
    auto it = m_models.find(monsterType);
    if (it == m_models.end()) {
        // Create model on first access
        MonsterModel& model = m_models[monsterType];
        
        // Determine model type and create
        if (monsterType == "Wolf" || monsterType == "Bear" || monsterType == "Lion") {
            createQuadrupedModel(monsterType, model);
        }
        else if (monsterType == "Dragon" || monsterType == "Wyvern") {
            createFlyingModel(monsterType, model);
        }
        else if (monsterType == "Slime" || monsterType == "Spider") {
            createSpecialModel(monsterType, model);
        }
        else {
            createBipedModel(monsterType, model);
        }
        
        return &model;
    }
    return &it->second;
}

void MonsterRenderer::renderMonster(VkCommandBuffer cmd,
                                    const std::string& monsterType,
                                    const glm::vec3& position,
                                    const glm::quat& rotation,
                                    const AnimationState& anim,
                                    float scale) {
    (void)cmd;
    (void)monsterType;
    (void)position;
    (void)rotation;
    (void)anim;
    (void)scale;
    
    // Actual rendering would bind buffers and draw
}

void MonsterRenderer::createVulkanBuffers(MonsterModel& model) {
    // Placeholder - actual Vulkan buffer creation
    (void)model;
}

void MonsterRenderer::destroyModel(MonsterModel& model) {
    if (model.vertexBuffer != VK_NULL_HANDLE) {
        vkDestroyBuffer(m_device, model.vertexBuffer, nullptr);
    }
    if (model.indexBuffer != VK_NULL_HANDLE) {
        vkDestroyBuffer(m_device, model.indexBuffer, nullptr);
    }
    if (model.vertexMemory != VK_NULL_HANDLE) {
        vkFreeMemory(m_device, model.vertexMemory, nullptr);
    }
    if (model.indexMemory != VK_NULL_HANDLE) {
        vkFreeMemory(m_device, model.indexMemory, nullptr);
    }
}

} // namespace clonemine
