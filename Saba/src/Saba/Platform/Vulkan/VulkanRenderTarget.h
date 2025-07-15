#pragma once

#include "Saba/Renderer/RenderTarget.h"
#include <vulkan/vulkan.h>

namespace Saba {

class VulkanRenderTarget : public RenderTarget {
public:
    VulkanRenderTarget(uint32_t width, uint32_t height, Format format, Usage usage, ClearValue clearVal);
    ~VulkanRenderTarget();

    void Resize(uint32_t width, uint32_t height) override;

    uint32_t GetWidth() const noexcept override { return m_Width; }
    uint32_t GetHeight() const noexcept override { return m_Height; }
    Format GetFormat() const noexcept override { return m_Format; }
    Usage GetUsage() const noexcept override { return m_Usage; }
    const ClearValue& GetClearValue() const noexcept override { return m_ClearValue; }
private:
    void CreateImage();
    void CreateImageView();
    void Destroy();
private:
    VkImage m_Image = nullptr;
    VkDeviceMemory m_ImageMemory = nullptr;
    VkImageView m_ImageView = nullptr;

    uint32_t m_Width, m_Height;
    Format m_Format;
    Usage m_Usage;
    ClearValue m_ClearValue;
};

} //namespace Saba
