#include <pch.h>
#include "VulkanRenderTarget.h"

#include "Saba/Platform/Vulkan/VulkanContext.h"

namespace Saba {

static VkFormat GetVulkanFormat(RenderTargetFormat format) {
    switch (format) {
        default: break;
        case RenderTargetFormat::R8:                return VK_FORMAT_R8_UINT;
        case RenderTargetFormat::R32F:              return VK_FORMAT_R32_SFLOAT;
        case RenderTargetFormat::R32_UINT:          return VK_FORMAT_R32_UINT;
        case RenderTargetFormat::RG8:               return VK_FORMAT_R8G8_UINT;
        case RenderTargetFormat::RG16F:             return VK_FORMAT_R16G16_SFLOAT;
        case RenderTargetFormat::RG32F:             return VK_FORMAT_R32G32_SFLOAT;
        case RenderTargetFormat::RG32_UINT:         return VK_FORMAT_R32G32_UINT;
        case RenderTargetFormat::RGB8:              return VK_FORMAT_R8G8B8_UINT;
        case RenderTargetFormat::RGB32F:            return VK_FORMAT_R32G32B32_SFLOAT;
        case RenderTargetFormat::RGB32_UINT:        return VK_FORMAT_R32G32B32_UINT;
        case RenderTargetFormat::RGBA8:             return VK_FORMAT_R8G8B8A8_UINT;
        case RenderTargetFormat::RGBA16F:           return VK_FORMAT_R16G16B16A16_SFLOAT;
        case RenderTargetFormat::RGBA32F:           return VK_FORMAT_R32G32B32A32_SFLOAT;
        case RenderTargetFormat::RGBA32_UINT:       return VK_FORMAT_R32G32B32A32_UINT;
        case RenderTargetFormat::Depth32F:          return VK_FORMAT_D32_SFLOAT;
        case RenderTargetFormat::Depth24Stencil8:   return VK_FORMAT_D24_UNORM_S8_UINT;
    }
    SB_CORE_THROW_INFO("Unknown render target format");
    return VK_FORMAT_UNDEFINED;
}
static VkImageUsageFlags GetVulkanAdditionalUsage(RenderTargetUsage additionalUsage) {
    VkImageUsageFlags usage = 0u;
    if ((static_cast<uint8_t>(additionalUsage) & static_cast<uint32_t>(RenderTargetUsage::CopySource)) != 0u) {
        usage |= VK_IMAGE_USAGE_TRANSFER_SRC_BIT;
    }
    if ((static_cast<uint8_t>(additionalUsage) & static_cast<uint32_t>(RenderTargetUsage::CopyDestination)) != 0u) {
        usage |= VK_IMAGE_USAGE_TRANSFER_DST_BIT;
    }
    if ((static_cast<uint8_t>(additionalUsage) & static_cast<uint32_t>(RenderTargetUsage::TextureAttachment)) != 0u) {
        usage |= VK_IMAGE_USAGE_SAMPLED_BIT;
    }
    return usage;
}

VulkanRenderTarget::VulkanRenderTarget(uint32_t width, uint32_t height, RenderTargetFormat format, RenderTargetUsage additionalUsage, glm::vec4 clearVal)
    : m_Format(format), m_AdditionalUsage(additionalUsage), m_Width(width), m_Height(height), m_ClearVal(clearVal)
{
    m_ImageCreateInfo.format = m_ImageViewCreateInfo.format = m_AttachmentDesc.format = GetVulkanFormat(m_Format);
    m_ImageCreateInfo.extent = { m_Width, m_Height, 1u };

    if (format == RenderTargetFormat::Depth32F) {
        m_ImageCreateInfo.usage = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
        m_ImageCreateInfo.initialLayout = VK_IMAGE_LAYOUT_DEPTH_ATTACHMENT_OPTIMAL;
        m_AttachmentDesc.finalLayout = VK_IMAGE_LAYOUT_DEPTH_ATTACHMENT_OPTIMAL;
    }
    else if (format == RenderTargetFormat::Depth24Stencil8) {
        m_ImageCreateInfo.usage = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
        m_ImageCreateInfo.initialLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
        m_AttachmentDesc.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
    }
    else {
        m_ImageCreateInfo.usage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
        m_ImageCreateInfo.initialLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
        m_AttachmentDesc.finalLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
    }
    m_ImageCreateInfo.usage |= GetVulkanAdditionalUsage(m_AdditionalUsage);

    Init();
}
VulkanRenderTarget::~VulkanRenderTarget() {
    auto context = VulkanContext::GetContextFromApplication();

    vkDestroyImageView(context->GetLogicalDevice(), m_ImageView, nullptr);
    vkDestroyImage(context->GetLogicalDevice(), m_Image, nullptr);
}
void VulkanRenderTarget::Init() {
    auto context = VulkanContext::GetContextFromApplication();

    m_ImageCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    m_ImageCreateInfo.imageType = VK_IMAGE_TYPE_2D;
    m_ImageCreateInfo.mipLevels = 1u;
    m_ImageCreateInfo.arrayLayers = 1u;
    m_ImageCreateInfo.samples = VK_SAMPLE_COUNT_1_BIT;
    m_ImageCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    SB_VK_GRAPHICS_CALL_INFO(vkCreateImage(context->GetLogicalDevice(), &m_ImageCreateInfo, nullptr, &m_Image),
        "Failed to create Vulkan image");

    m_ImageViewCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    m_ImageViewCreateInfo.image = m_Image;
    m_ImageViewCreateInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
    m_ImageViewCreateInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
    m_ImageViewCreateInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
    m_ImageViewCreateInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
    m_ImageViewCreateInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;

    if (m_Format == RenderTargetFormat::Depth32F)
        m_ImageViewCreateInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;
    else if (m_Format == RenderTargetFormat::Depth24Stencil8)
        m_ImageViewCreateInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT | VK_IMAGE_ASPECT_STENCIL_BIT;
    else
        m_ImageViewCreateInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;

    m_ImageViewCreateInfo.subresourceRange.baseMipLevel = 0;
    m_ImageViewCreateInfo.subresourceRange.levelCount = 1;
    m_ImageViewCreateInfo.subresourceRange.baseArrayLayer = 0;
    m_ImageViewCreateInfo.subresourceRange.layerCount = 1;

    SB_VK_GRAPHICS_CALL_INFO(vkCreateImageView(context->GetLogicalDevice(), &m_ImageViewCreateInfo  , nullptr, &m_ImageView),
        "Failed to create Vulkan image view");

    m_AttachmentDesc.samples = m_ImageCreateInfo.samples;
    m_AttachmentDesc.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    m_AttachmentDesc.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    if (m_Format == RenderTargetFormat::Depth24Stencil8) {
        m_AttachmentDesc.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
        m_AttachmentDesc.stencilStoreOp = VK_ATTACHMENT_STORE_OP_STORE;
    }
    else {
        m_AttachmentDesc.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        m_AttachmentDesc.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    }
    m_AttachmentDesc.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
}

}