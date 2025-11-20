#pragma once

#include <vulkan/vulkan.h>
#include <vector>
#include <string>
#include <memory>

namespace clonemine {

class VulkanDevice;

class VulkanPipeline {
public:
    VulkanPipeline(const VulkanDevice& device, VkRenderPass renderPass, VkExtent2D extent);
    ~VulkanPipeline();

    // Delete copy operations
    VulkanPipeline(const VulkanPipeline&) = delete;
    VulkanPipeline& operator=(const VulkanPipeline&) = delete;

    [[nodiscard]] VkPipeline getPipeline() const noexcept { return m_pipeline; }
    [[nodiscard]] VkPipelineLayout getLayout() const noexcept { return m_pipelineLayout; }

private:
    void createPipeline(VkRenderPass renderPass, VkExtent2D extent);
    VkShaderModule createShaderModule(const std::vector<char>& code) const;
    std::vector<char> readFile(const std::string& filename) const;

    const VulkanDevice& m_device;
    VkPipeline m_pipeline{VK_NULL_HANDLE};
    VkPipelineLayout m_pipelineLayout{VK_NULL_HANDLE};
};

} // namespace clonemine
