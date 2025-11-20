#pragma once

#include <vulkan/vulkan.h>
#include <vector>
#include <memory>
#include <string_view>

namespace clonemine {

class Window;

class VulkanContext {
public:
    explicit VulkanContext(const Window& window);
    ~VulkanContext();

    // Delete copy operations
    VulkanContext(const VulkanContext&) = delete;
    VulkanContext& operator=(const VulkanContext&) = delete;

    [[nodiscard]] VkInstance getInstance() const noexcept { return m_instance; }
    [[nodiscard]] VkSurfaceKHR getSurface() const noexcept { return m_surface; }

private:
    void createInstance();
    void createSurface(const Window& window);
    void setupDebugMessenger();
    
    bool checkValidationLayerSupport() const;
    std::vector<const char*> getRequiredExtensions() const;

    VkInstance m_instance{VK_NULL_HANDLE};
    VkSurfaceKHR m_surface{VK_NULL_HANDLE};
    VkDebugUtilsMessengerEXT m_debugMessenger{VK_NULL_HANDLE};

#ifdef NDEBUG
    static constexpr bool enableValidationLayers = false;
#else
    static constexpr bool enableValidationLayers = true;
#endif
};

} // namespace clonemine
