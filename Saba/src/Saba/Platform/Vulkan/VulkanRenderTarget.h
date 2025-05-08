#pragma once

#include "Saba/Renderer/RenderTarget.h"

#include <vulkan/vulkan.h>

namespace Saba {

class VulkanRenderTarget : public RenderTarget {
public:
    VulkanRenderTarget(uint32_t width, uint32_t height, Format format = Format::RGBA8,
        Usage usage = Usage::ColorAttachment, ClearValue clearValue = { glm::vec4(0.0f, 0.0f, 0.0f, 1.0f) });
    ~VulkanRenderTarget();

    uint32_t GetWidth() const noexcept override { return m_Width; }
    uint32_t GetHeight() const noexcept override { return m_Height; }

    void Resize(uint32_t width, uint32_t height);

    VkImageView GetImageView() const noexcept { return m_ImageView; }

    const VkAttachmentDescription& GetAttachmentDescription() const noexcept { return m_AttachmentDescription; }

    Format GetFormat() const noexcept { return m_Format; }
    Usage GetUsage() const noexcept { return m_Usage; }
    ClearValue GetClearVal() const noexcept { return m_ClearVal; }
private:
    void Validate();
    void Init();

    void CreateVulkanObjects();
    void DestroyVulkanObjects();

    bool HasDepthStencilFormat() { return m_Format == Format::Depth24Stencil8 || m_Format == Format::Depth32F; }
private:
    VkImage m_Image = nullptr;
    VkDeviceMemory m_ImageMemory = nullptr;
    VkImageView m_ImageView = nullptr;

    VkImageCreateInfo m_ImageCreateInfo{};
    VkImageViewCreateInfo m_ImageViewCreateInfo{};
    VkAttachmentDescription m_AttachmentDescription{};

    uint32_t m_Width, m_Height;
    ClearValue m_ClearVal;
    Format m_Format;
    Usage m_Usage;
};

}
