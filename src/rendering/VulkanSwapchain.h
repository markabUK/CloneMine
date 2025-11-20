#pragma once

#include <vulkan/vulkan.h>
#include <vector>
#include <memory>

namespace clonemine {

class VulkanDevice;
class Window;

struct SwapchainSupportDetails {
    VkSurfaceCapabilitiesKHR capabilities;
    std::vector<VkSurfaceFormatKHR> formats;
    std::vector<VkPresentModeKHR> presentModes;
};

class VulkanSwapchain {
public:
    VulkanSwapchain(const VulkanDevice& device, const Window& window, VkSurfaceKHR surface);
    ~VulkanSwapchain();

    // Delete copy operations
    VulkanSwapchain(const VulkanSwapchain&) = delete;
    VulkanSwapchain& operator=(const VulkanSwapchain&) = delete;

    void recreate(const Window& window);
    
    [[nodiscard]] VkSwapchainKHR getSwapchain() const noexcept { return m_swapchain; }
    [[nodiscard]] VkFormat getImageFormat() const noexcept { return m_imageFormat; }
    [[nodiscard]] VkExtent2D getExtent() const noexcept { return m_extent; }
    [[nodiscard]] const std::vector<VkImageView>& getImageViews() const noexcept { return m_imageViews; }
    [[nodiscard]] size_t getImageCount() const noexcept { return m_images.size(); }

private:
    void createSwapchain(const Window& window);
    void createImageViews();
    void cleanup();
    
    SwapchainSupportDetails querySwapchainSupport(VkPhysicalDevice device) const;
    VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& formats) const;
    VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& presentModes) const;
    VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities, const Window& window) const;

    const VulkanDevice& m_device;
    VkSurfaceKHR m_surface;
    VkSwapchainKHR m_swapchain{VK_NULL_HANDLE};
    std::vector<VkImage> m_images;
    std::vector<VkImageView> m_imageViews;
    VkFormat m_imageFormat;
    VkExtent2D m_extent;
};

} // namespace clonemine
