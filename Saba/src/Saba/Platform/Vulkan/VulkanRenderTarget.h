#pragma once

#include "Saba/Renderer/RenderTarget.h"

#include <vulkan/vulkan.h>

namespace Saba {

class VulkanRenderTarget : public RenderTarget {
public:
    VulkanRenderTarget(uint32_t width, uint32_t height, RenderTargetFormat format, RenderTargetUsage additionalUsage, glm::vec4 clearVal);
    ~VulkanRenderTarget();
private:
    void Init();
private:
    VkImage m_Image = nullptr;
    VkImageCreateInfo m_ImageCreateInfo{};

    VkImageView m_ImageView = nullptr;
    VkImageViewCreateInfo m_ImageViewCreateInfo{};

    RenderTargetFormat m_Format;
    RenderTargetUsage m_AdditionalUsage;
    uint32_t m_Width, m_Height;

    VkAttachmentDescription m_AttachmentDesc{};

    glm::vec4 m_ClearVal;
};

}