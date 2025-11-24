#include "EffectRenderer.h"
#include <algorithm>

namespace clonemine {

EffectRenderer::EffectRenderer() {
    m_particles.reserve(10000);
}

EffectRenderer::~EffectRenderer() = default;

void EffectRenderer::renderParticles(VkCommandBuffer cmdBuffer) {
    if (m_particles.empty()) return;
    
    buildInstanceBuffer();
    // Bind particle shader pipeline
    // Draw instanced particles (all in one draw call)
}

void EffectRenderer::updateParticles(float deltaTime) {
    // Update all particles
    for (auto& particle : m_particles) {
        particle.position += particle.velocity * deltaTime;
        particle.lifetime += deltaTime;
        
        // Fade alpha as particle ages
        float lifePercent = particle.lifetime / particle.maxLifetime;
        particle.color.a = 1.0f - lifePercent;
    }
    
    // Remove dead particles
    m_particles.erase(
        std::remove_if(m_particles.begin(), m_particles.end(),
            [](const Particle& p) { return p.lifetime >= p.maxLifetime; }),
        m_particles.end()
    );
}

void EffectRenderer::spawnFireball(glm::vec3 start, glm::vec3 target) {
    glm::vec3 direction = glm::normalize(target - start);
    
    for (int i = 0; i < 20; ++i) {
        Particle p;
        p.position = start;
        p.velocity = direction * 15.0f + glm::vec3(
            (rand() % 100 - 50) / 100.0f,
            (rand() % 100 - 50) / 100.0f,
            (rand() % 100 - 50) / 100.0f
        );
        p.color = glm::vec4(1.0f, 0.5f, 0.0f, 1.0f); // Orange
        p.lifetime = 0.0f;
        p.maxLifetime = 1.0f;
        p.size = 0.3f;
        m_particles.push_back(p);
    }
}

void EffectRenderer::spawnFrostbolt(glm::vec3 start, glm::vec3 target) {
    glm::vec3 direction = glm::normalize(target - start);
    
    for (int i = 0; i < 15; ++i) {
        Particle p;
        p.position = start;
        p.velocity = direction * 12.0f;
        p.color = glm::vec4(0.5f, 0.7f, 1.0f, 1.0f); // Light blue
        p.lifetime = 0.0f;
        p.maxLifetime = 1.2f;
        p.size = 0.25f;
        m_particles.push_back(p);
    }
}

void EffectRenderer::spawnHealingEffect(glm::vec3 position) {
    for (int i = 0; i < 30; ++i) {
        Particle p;
        p.position = position;
        p.velocity = glm::vec3(0.0f, 2.0f, 0.0f); // Rise upward
        p.color = glm::vec4(0.2f, 1.0f, 0.3f, 1.0f); // Green
        p.lifetime = 0.0f;
        p.maxLifetime = 2.0f;
        p.size = 0.2f;
        m_particles.push_back(p);
    }
}

void EffectRenderer::spawnExplosion(glm::vec3 position, float radius) {
    for (int i = 0; i < 50; ++i) {
        Particle p;
        p.position = position;
        float angle = (rand() % 360) * 3.14159f / 180.0f;
        float speed = 5.0f + (rand() % 100) / 20.0f;
        p.velocity = glm::vec3(
            cos(angle) * speed,
            (rand() % 100) / 50.0f,
            sin(angle) * speed
        );
        p.color = glm::vec4(1.0f, 0.3f, 0.0f, 1.0f); // Red-orange
        p.lifetime = 0.0f;
        p.maxLifetime = 0.8f;
        p.size = 0.4f;
        m_particles.push_back(p);
    }
}

void EffectRenderer::renderGroundCircle(glm::vec3 center, float radius, glm::vec4 color) {
    // Render AOE circle on ground using triangle fan
}

void EffectRenderer::renderAura(glm::vec3 center, ElementalDamageType type) {
    // Spawn orbiting particles around center point
    // Color based on damage type
}

void EffectRenderer::buildInstanceBuffer() {
    // Build instance buffer for GPU with all particle transforms
}

} // namespace clonemine
