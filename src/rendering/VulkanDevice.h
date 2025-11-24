#pragma once

#include <vulkan/vulkan.h>
#include <vector>
#include <optional>

namespace clonemine {

class VulkanContext;

struct QueueFamilyIndices {
    std::optional<uint32_t> graphicsFamily;
    std::optional<uint32_t> presentFamily;

    [[nodiscard]] bool isComplete() const noexcept {
        return graphicsFamily.has_value() && presentFamily.has_value();
    }
};

class VulkanDevice {
public:
    explicit VulkanDevice(const VulkanContext& context);
    ~VulkanDevice();

    // Delete copy operations
    VulkanDevice(const VulkanDevice&) = delete;
    VulkanDevice& operator=(const VulkanDevice&) = delete;

    [[nodiscard]] VkPhysicalDevice getPhysicalDevice() const noexcept { return m_physicalDevice; }
    [[nodiscard]] VkDevice getDevice() const noexcept { return m_device; }
    [[nodiscard]] VkQueue getGraphicsQueue() const noexcept { return m_graphicsQueue; }
    [[nodiscard]] VkQueue getPresentQueue() const noexcept { return m_presentQueue; }
    [[nodiscard]] const QueueFamilyIndices& getQueueFamilies() const noexcept { return m_queueFamilies; }

    void waitIdle() const;

private:
    void pickPhysicalDevice(const VulkanContext& context);
    void createLogicalDevice();
    
    bool isDeviceSuitable(VkPhysicalDevice device, VkSurfaceKHR surface) const;
    QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device, VkSurfaceKHR surface) const;

    VkPhysicalDevice m_physicalDevice{VK_NULL_HANDLE};
    VkDevice m_device{VK_NULL_HANDLE};
    VkQueue m_graphicsQueue{VK_NULL_HANDLE};
    VkQueue m_presentQueue{VK_NULL_HANDLE};
    QueueFamilyIndices m_queueFamilies;
    VkSurfaceKHR m_surface{VK_NULL_HANDLE};
};

} // namespace clonemine
