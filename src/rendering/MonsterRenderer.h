#pragma once

#include <vulkan/vulkan.h>
#include <glm/glm.hpp>
#include <vector>
#include <string>

namespace clonemine {

enum class MonsterModelType {
    BIPED,      // Humanoid (Goblin, Zombie, Skeleton)
    QUADRUPED,  // Four-legged (Wolf, Bear, Lion)
    FLYING,     // Winged (Dragon, Wyvern, Harpy)
    SPECIAL     // Unique models (Slime, Spider)
};

struct MonsterVertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texCoord;
    glm::vec3 color;
};

struct MonsterModel {
    std::vector<MonsterVertex> vertices;
    std::vector<uint32_t> indices;
    VkBuffer vertexBuffer = VK_NULL_HANDLE;
    VkBuffer indexBuffer = VK_NULL_HANDLE;
    VkDeviceMemory vertexMemory = VK_NULL_HANDLE;
    VkDeviceMemory indexMemory = VK_NULL_HANDLE;
    MonsterModelType type;
};

enum class MonsterAnimation {
    IDLE,
    WALK,
    RUN,
    ATTACK,
    DAMAGE,
    DEATH,
    SPECIAL
};

struct AnimationState {
    MonsterAnimation current = MonsterAnimation::IDLE;
    float time = 0.0f;
    float speed = 1.0f;
    bool looping = true;
};

class MonsterRenderer {
public:
    MonsterRenderer(VkDevice device, VkPhysicalDevice physicalDevice);
    ~MonsterRenderer();

    // Create models for different monster types
    void createBipedModel(const std::string& monsterType, MonsterModel& model);
    void createQuadrupedModel(const std::string& monsterType, MonsterModel& model);
    void createFlyingModel(const std::string& monsterType, MonsterModel& model);
    void createSpecialModel(const std::string& monsterType, MonsterModel& model);
    
    // Render a monster instance
    void renderMonster(VkCommandBuffer cmd, const std::string& monsterType,
                      const glm::vec3& position, const glm::quat& rotation,
                      const AnimationState& anim, float scale = 1.0f);
    
    // Update animation
    void updateAnimation(AnimationState& anim, float deltaTime);
    
    // Apply animation to model (skeletal animation)
    void applyAnimation(const MonsterModel& model, const AnimationState& anim,
                       std::vector<glm::mat4>& boneTransforms);
    
    // Get model for monster type
    MonsterModel* getModel(const std::string& monsterType);
    
    // Create blocky model parts (head, body, limbs)
    void createBlockyHead(std::vector<MonsterVertex>& verts, std::vector<uint32_t>& indices,
                         const glm::vec3& position, const glm::vec3& size,
                         const glm::vec3& color);
    
    void createBlockyBody(std::vector<MonsterVertex>& verts, std::vector<uint32_t>& indices,
                         const glm::vec3& position, const glm::vec3& size,
                         const glm::vec3& color);
    
    void createBlockyLimb(std::vector<MonsterVertex>& verts, std::vector<uint32_t>& indices,
                         const glm::vec3& position, const glm::vec3& size,
                         const glm::vec3& color);

private:
    void createVulkanBuffers(MonsterModel& model);
    void destroyModel(MonsterModel& model);
    
    // Add a cube to the model
    void addCube(std::vector<MonsterVertex>& verts, std::vector<uint32_t>& indices,
                const glm::vec3& center, const glm::vec3& size, const glm::vec3& color);
    
    VkDevice m_device;
    VkPhysicalDevice m_physicalDevice;
    std::unordered_map<std::string, MonsterModel> m_models;
};

} // namespace clonemine
