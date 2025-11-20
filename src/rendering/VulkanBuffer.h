#pragma once

#include <vulkan/vulkan.h>
#include <cstddef>

namespace clonemine {

class VulkanDevice;

class VulkanBuffer {
public:
    VulkanBuffer(const VulkanDevice& device, VkDeviceSize size, 
                 VkBufferUsageFlags usage, VkMemoryPropertyFlags properties);
    ~VulkanBuffer();

    // Delete copy operations
    VulkanBuffer(const VulkanBuffer&) = delete;
    VulkanBuffer& operator=(const VulkanBuffer&) = delete;
    
    // Allow move operations
    VulkanBuffer(VulkanBuffer&& other) noexcept;
    VulkanBuffer& operator=(VulkanBuffer&& other) noexcept;

    void map(void** data);
    void unmap();
    void copyFrom(const void* data, VkDeviceSize size);

    [[nodiscard]] VkBuffer getBuffer() const noexcept { return m_buffer; }
    [[nodiscard]] VkDeviceMemory getMemory() const noexcept { return m_memory; }
    [[nodiscard]] VkDeviceSize getSize() const noexcept { return m_size; }

private:
    uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties) const;

    const VulkanDevice& m_device;
    VkBuffer m_buffer{VK_NULL_HANDLE};
    VkDeviceMemory m_memory{VK_NULL_HANDLE};
    VkDeviceSize m_size;
};

} // namespace clonemine
