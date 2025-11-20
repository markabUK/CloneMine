#include "rendering/VulkanBuffer.h"
#include "rendering/VulkanDevice.h"
#include <stdexcept>
#include <cstring>

namespace clonemine {

VulkanBuffer::VulkanBuffer(const VulkanDevice& device, VkDeviceSize size,
                           VkBufferUsageFlags usage, VkMemoryPropertyFlags properties)
    : m_device(device), m_size(size)
{
    VkBufferCreateInfo bufferInfo{};
    bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferInfo.size = size;
    bufferInfo.usage = usage;
    bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    if (vkCreateBuffer(m_device.getDevice(), &bufferInfo, nullptr, &m_buffer) != VK_SUCCESS) {
        throw std::runtime_error("Failed to create buffer!");
    }

    VkMemoryRequirements memRequirements;
    vkGetBufferMemoryRequirements(m_device.getDevice(), m_buffer, &memRequirements);

    VkMemoryAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memRequirements.size;
    allocInfo.memoryTypeIndex = findMemoryType(memRequirements.memoryTypeBits, properties);

    if (vkAllocateMemory(m_device.getDevice(), &allocInfo, nullptr, &m_memory) != VK_SUCCESS) {
        throw std::runtime_error("Failed to allocate buffer memory!");
    }

    vkBindBufferMemory(m_device.getDevice(), m_buffer, m_memory, 0);
}

VulkanBuffer::~VulkanBuffer() {
    if (m_buffer != VK_NULL_HANDLE) {
        vkDestroyBuffer(m_device.getDevice(), m_buffer, nullptr);
    }
    if (m_memory != VK_NULL_HANDLE) {
        vkFreeMemory(m_device.getDevice(), m_memory, nullptr);
    }
}

VulkanBuffer::VulkanBuffer(VulkanBuffer&& other) noexcept
    : m_device(other.m_device)
    , m_buffer(other.m_buffer)
    , m_memory(other.m_memory)
    , m_size(other.m_size)
{
    other.m_buffer = VK_NULL_HANDLE;
    other.m_memory = VK_NULL_HANDLE;
}

VulkanBuffer& VulkanBuffer::operator=(VulkanBuffer&& other) noexcept {
    if (this != &other) {
        if (m_buffer != VK_NULL_HANDLE) {
            vkDestroyBuffer(m_device.getDevice(), m_buffer, nullptr);
        }
        if (m_memory != VK_NULL_HANDLE) {
            vkFreeMemory(m_device.getDevice(), m_memory, nullptr);
        }

        m_buffer = other.m_buffer;
        m_memory = other.m_memory;
        m_size = other.m_size;

        other.m_buffer = VK_NULL_HANDLE;
        other.m_memory = VK_NULL_HANDLE;
    }
    return *this;
}

void VulkanBuffer::map(void** data) {
    vkMapMemory(m_device.getDevice(), m_memory, 0, m_size, 0, data);
}

void VulkanBuffer::unmap() {
    vkUnmapMemory(m_device.getDevice(), m_memory);
}

void VulkanBuffer::copyFrom(const void* data, VkDeviceSize size) {
    void* mapped;
    map(&mapped);
    std::memcpy(mapped, data, static_cast<size_t>(size));
    unmap();
}

uint32_t VulkanBuffer::findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties) const {
    VkPhysicalDeviceMemoryProperties memProperties;
    vkGetPhysicalDeviceMemoryProperties(m_device.getPhysicalDevice(), &memProperties);

    for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
        if ((typeFilter & (1 << i)) && 
            (memProperties.memoryTypes[i].propertyFlags & properties) == properties) {
            return i;
        }
    }

    throw std::runtime_error("Failed to find suitable memory type!");
}

} // namespace clonemine
