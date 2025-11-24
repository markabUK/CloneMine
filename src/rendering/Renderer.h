#pragma once

#include <memory>

namespace clonemine {

class Window;
class VulkanContext;
class VulkanDevice;
class VulkanSwapchain;
class VulkanPipeline;
class World;

class Renderer {
public:
    explicit Renderer(const Window& window);
    ~Renderer();

    // Delete copy operations
    Renderer(const Renderer&) = delete;
    Renderer& operator=(const Renderer&) = delete;

    void beginFrame();
    void endFrame();
    void renderWorld(const World& world);
    void handleResize();
    void waitIdle() const;

private:
    void createRenderPass();
    void createFramebuffers();
    void createCommandPool();
    void createCommandBuffers();
    void createSyncObjects();
    void recordCommandBuffer(uint32_t imageIndex);

    const Window& m_window;
    std::unique_ptr<VulkanContext> m_context;
    std::unique_ptr<VulkanDevice> m_device;
    std::unique_ptr<VulkanSwapchain> m_swapchain;
    std::unique_ptr<VulkanPipeline> m_pipeline;

    VkRenderPass m_renderPass{VK_NULL_HANDLE};
    std::vector<VkFramebuffer> m_framebuffers;
    VkCommandPool m_commandPool{VK_NULL_HANDLE};
    std::vector<VkCommandBuffer> m_commandBuffers;
    
    std::vector<VkSemaphore> m_imageAvailableSemaphores;
    std::vector<VkSemaphore> m_renderFinishedSemaphores;
    std::vector<VkFence> m_inFlightFences;
    
    uint32_t m_currentFrame{0};
    static constexpr size_t MAX_FRAMES_IN_FLIGHT = 2;
};

} // namespace clonemine
