#pragma once

#include <vulkan/vulkan.h>
#include <glm/glm.hpp>
#include <vector>
#include "../combat/DamageTypes.h"

namespace clonemine {

struct Particle {
    glm::vec3 position;
    glm::vec3 velocity;
    glm::vec4 color;
    float lifetime;
    float maxLifetime;
    float size;
};

class EffectRenderer {
public:
    EffectRenderer();
    ~EffectRenderer();
    
    void renderParticles(VkCommandBuffer cmdBuffer);
    void updateParticles(float deltaTime);
    
    void spawnFireball(glm::vec3 start, glm::vec3 target);
    void spawnFrostbolt(glm::vec3 start, glm::vec3 target);
    void spawnHealingEffect(glm::vec3 position);
    void spawnExplosion(glm::vec3 position, float radius);
    
    void renderGroundCircle(glm::vec3 center, float radius, glm::vec4 color);
    void renderAura(glm::vec3 center, ElementalDamageType type);
    
    void buildInstanceBuffer();
    
private:
    std::vector<Particle> m_particles;
    VkBuffer m_instanceBuffer = VK_NULL_HANDLE;
    VkDeviceMemory m_instanceMemory = VK_NULL_HANDLE;
};

} // namespace clonemine
