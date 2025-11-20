#include "rendering/VulkanSwapchain.h"
#include "rendering/VulkanDevice.h"
#include "core/Window.h"
#include <algorithm>
#include <stdexcept>
#include <limits>

namespace clonemine {

VulkanSwapchain::VulkanSwapchain(const VulkanDevice& device, const Window& window, VkSurfaceKHR surface)
    : m_device(device), m_surface(surface)
{
    createSwapchain(window);
    createImageViews();
}

VulkanSwapchain::~VulkanSwapchain() {
    cleanup();
}

void VulkanSwapchain::recreate(const Window& window) {
    m_device.waitIdle();
    cleanup();
    createSwapchain(window);
    createImageViews();
}

void VulkanSwapchain::createSwapchain(const Window& window) {
    SwapchainSupportDetails swapchainSupport = querySwapchainSupport(m_device.getPhysicalDevice());

    VkSurfaceFormatKHR surfaceFormat = chooseSwapSurfaceFormat(swapchainSupport.formats);
    VkPresentModeKHR presentMode = chooseSwapPresentMode(swapchainSupport.presentModes);
    VkExtent2D extent = chooseSwapExtent(swapchainSupport.capabilities, window);

    uint32_t imageCount = swapchainSupport.capabilities.minImageCount + 1;
    if (swapchainSupport.capabilities.maxImageCount > 0 && 
        imageCount > swapchainSupport.capabilities.maxImageCount) {
        imageCount = swapchainSupport.capabilities.maxImageCount;
    }

    VkSwapchainCreateInfoKHR createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    createInfo.surface = m_surface;
    createInfo.minImageCount = imageCount;
    createInfo.imageFormat = surfaceFormat.format;
    createInfo.imageColorSpace = surfaceFormat.colorSpace;
    createInfo.imageExtent = extent;
    createInfo.imageArrayLayers = 1;
    createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

    auto indices = m_device.getQueueFamilies();
    uint32_t queueFamilyIndices[] = {
        indices.graphicsFamily.value(),
        indices.presentFamily.value()
    };

    if (indices.graphicsFamily != indices.presentFamily) {
        createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
        createInfo.queueFamilyIndexCount = 2;
        createInfo.pQueueFamilyIndices = queueFamilyIndices;
    } else {
        createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
    }

    createInfo.preTransform = swapchainSupport.capabilities.currentTransform;
    createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    createInfo.presentMode = presentMode;
    createInfo.clipped = VK_TRUE;
    createInfo.oldSwapchain = VK_NULL_HANDLE;

    if (vkCreateSwapchainKHR(m_device.getDevice(), &createInfo, nullptr, &m_swapchain) != VK_SUCCESS) {
        throw std::runtime_error("Failed to create swap chain!");
    }

    vkGetSwapchainImagesKHR(m_device.getDevice(), m_swapchain, &imageCount, nullptr);
    m_images.resize(imageCount);
    vkGetSwapchainImagesKHR(m_device.getDevice(), m_swapchain, &imageCount, m_images.data());

    m_imageFormat = surfaceFormat.format;
    m_extent = extent;
}

void VulkanSwapchain::createImageViews() {
    m_imageViews.resize(m_images.size());

    for (size_t i = 0; i < m_images.size(); i++) {
        VkImageViewCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        createInfo.image = m_images[i];
        createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
        createInfo.format = m_imageFormat;
        createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        createInfo.subresourceRange.baseMipLevel = 0;
        createInfo.subresourceRange.levelCount = 1;
        createInfo.subresourceRange.baseArrayLayer = 0;
        createInfo.subresourceRange.layerCount = 1;

        if (vkCreateImageView(m_device.getDevice(), &createInfo, nullptr, &m_imageViews[i]) != VK_SUCCESS) {
            throw std::runtime_error("Failed to create image views!");
        }
    }
}

void VulkanSwapchain::cleanup() {
    for (auto imageView : m_imageViews) {
        vkDestroyImageView(m_device.getDevice(), imageView, nullptr);
    }
    m_imageViews.clear();

    if (m_swapchain != VK_NULL_HANDLE) {
        vkDestroySwapchainKHR(m_device.getDevice(), m_swapchain, nullptr);
        m_swapchain = VK_NULL_HANDLE;
    }
}

SwapchainSupportDetails VulkanSwapchain::querySwapchainSupport(VkPhysicalDevice device) const {
    SwapchainSupportDetails details;

    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, m_surface, &details.capabilities);

    uint32_t formatCount;
    vkGetPhysicalDeviceSurfaceFormatsKHR(device, m_surface, &formatCount, nullptr);
    if (formatCount != 0) {
        details.formats.resize(formatCount);
        vkGetPhysicalDeviceSurfaceFormatsKHR(device, m_surface, &formatCount, details.formats.data());
    }

    uint32_t presentModeCount;
    vkGetPhysicalDeviceSurfacePresentModesKHR(device, m_surface, &presentModeCount, nullptr);
    if (presentModeCount != 0) {
        details.presentModes.resize(presentModeCount);
        vkGetPhysicalDeviceSurfacePresentModesKHR(device, m_surface, &presentModeCount, details.presentModes.data());
    }

    return details;
}

VkSurfaceFormatKHR VulkanSwapchain::chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& formats) const {
    for (const auto& format : formats) {
        if (format.format == VK_FORMAT_B8G8R8A8_SRGB && 
            format.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
            return format;
        }
    }
    return formats[0];
}

VkPresentModeKHR VulkanSwapchain::chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& presentModes) const {
    for (const auto& mode : presentModes) {
        if (mode == VK_PRESENT_MODE_MAILBOX_KHR) {
            return mode;
        }
    }
    return VK_PRESENT_MODE_FIFO_KHR;
}

VkExtent2D VulkanSwapchain::chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities, const Window& window) const {
    if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max()) {
        return capabilities.currentExtent;
    }

    VkExtent2D actualExtent = {
        window.getWidth(),
        window.getHeight()
    };

    actualExtent.width = std::clamp(actualExtent.width, 
        capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
    actualExtent.height = std::clamp(actualExtent.height,
        capabilities.minImageExtent.height, capabilities.maxImageExtent.height);

    return actualExtent;
}

} // namespace clonemine
