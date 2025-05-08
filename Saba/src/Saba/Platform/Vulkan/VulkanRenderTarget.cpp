#include <pch.h>
#include "VulkanRenderTarget.h"

#include "Saba/Platform/Vulkan/VulkanContext.h"

namespace Saba {

static VkFormat GetVulkanFormat(RenderTarget::Format format) {
    switch (format) {
        case RenderTarget::Format::R8:                  return VK_FORMAT_R8_UINT;
        case RenderTarget::Format::R32F:                return VK_FORMAT_R32_SFLOAT;
        case RenderTarget::Format::R32_UINT:            return VK_FORMAT_R32_UINT;
        case RenderTarget::Format::RG8:                 return VK_FORMAT_R8G8_UINT;
        case RenderTarget::Format::RG16F:               return VK_FORMAT_R16G16_SFLOAT;
        case RenderTarget::Format::RG32F:               return VK_FORMAT_R32G32_SFLOAT;
        case RenderTarget::Format::RG32_UINT:           return VK_FORMAT_R32G32_UINT;
        case RenderTarget::Format::RGB8:                return VK_FORMAT_R8G8B8_UINT;
        case RenderTarget::Format::RGB32F:              return VK_FORMAT_R32G32B32_SFLOAT;
        case RenderTarget::Format::RGB32_UINT:          return VK_FORMAT_R32G32B32_UINT;
        case RenderTarget::Format::RGBA8:               return VK_FORMAT_R8G8B8A8_UINT;
        case RenderTarget::Format::RGBA16F:             return VK_FORMAT_R16G16B16A16_SFLOAT;
        case RenderTarget::Format::RGBA32F:             return VK_FORMAT_R32G32B32A32_SFLOAT;
        case RenderTarget::Format::RGBA32_UINT:         return VK_FORMAT_R32G32B32A32_UINT;
        case RenderTarget::Format::Depth32F:            return VK_FORMAT_D32_SFLOAT;
        case RenderTarget::Format::Depth24Stencil8:     return VK_FORMAT_D24_UNORM_S8_UINT;
        default:
            SB_CORE_THROW_INFO("Unknown render target format");
            return VK_FORMAT_UNDEFINED;
    }
}
static VkImageUsageFlags GetVulkanUsage(RenderTarget::Usage usage) {
    VkImageUsageFlags vulkanUsage = 0u;
    if ((usage & RenderTarget::Usage::DepthStencilAttachment) != 0u)
        vulkanUsage |= VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
    else
        vulkanUsage |= VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

    if ((usage & RenderTarget::Usage::CopySource) != 0u)
        vulkanUsage |= VK_IMAGE_USAGE_TRANSFER_SRC_BIT;
    if ((usage & RenderTarget::Usage::TextureAttachment) != 0u)
        vulkanUsage |= VK_IMAGE_USAGE_SAMPLED_BIT;
    if ((usage & RenderTarget::Usage::InputAttachment) != 0u)
        vulkanUsage |= VK_IMAGE_USAGE_INPUT_ATTACHMENT_BIT;
    return vulkanUsage;
}

VulkanRenderTarget::VulkanRenderTarget(uint32_t width, uint32_t height, Format format, Usage usage, ClearValue clearValue)
    : m_Width(width), m_Height(height), m_ClearVal(clearValue), m_Format(format), m_Usage(usage)
{
    Validate();
    Init();
}
VulkanRenderTarget::~VulkanRenderTarget() {
    DestroyVulkanObjects();
}

void VulkanRenderTarget::Validate() {
    SB_CORE_ASSERT(m_Width > 0u && m_Height > 0u, "Render target must have non 0 width and height");
    SB_CORE_ASSERT(m_Width <= 8196u && m_Height <= 8196u, "Render target must have width and height of maximum 8196 pixels");

    SB_CORE_ASSERT(m_Format >= Format::First && m_Format <= Format::Last, "Invalid render target format");

    if ((m_Usage & Usage::DepthStencilAttachment) != 0u) {
        SB_CORE_ASSERT(HasDepthStencilFormat(),
            "Render target with usage DepthStencilAttachment must have format of Depth32F or Depth24Stencil8");
    }
    else {
        SB_CORE_ASSERT(!HasDepthStencilFormat(),
            "Render target with usage DepthStencilAttachment cannot have format of Depth32F or Depth24Stencil8");
    }
}

void VulkanRenderTarget::Init() {
    m_ImageCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    m_ImageCreateInfo.imageType = VK_IMAGE_TYPE_2D;
    m_ImageCreateInfo.format = GetVulkanFormat(m_Format);
    m_ImageCreateInfo.mipLevels = 1u;
    m_ImageCreateInfo.arrayLayers = 1u;
    m_ImageCreateInfo.samples = VK_SAMPLE_COUNT_1_BIT;
    m_ImageCreateInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
    m_ImageCreateInfo.usage = GetVulkanUsage(m_Usage);
    m_ImageCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    m_ImageCreateInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;

    m_ImageViewCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    m_ImageViewCreateInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
    m_ImageViewCreateInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
    m_ImageViewCreateInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
    m_ImageViewCreateInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
    m_ImageViewCreateInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;

    if (m_Format == Format::Depth32F)
        m_ImageViewCreateInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;
    else if (m_Format == Format::Depth24Stencil8)
        m_ImageViewCreateInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT | VK_IMAGE_ASPECT_STENCIL_BIT;
    else
        m_ImageViewCreateInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;

    m_ImageViewCreateInfo.subresourceRange.baseMipLevel = 0u;
    m_ImageViewCreateInfo.subresourceRange.levelCount = 1u;
    m_ImageViewCreateInfo.subresourceRange.baseArrayLayer = 0u;
    m_ImageViewCreateInfo.subresourceRange.layerCount = 1u;

    m_AttachmentDescription.format = m_ImageCreateInfo.format;
    m_AttachmentDescription.samples = VK_SAMPLE_COUNT_1_BIT;
    m_AttachmentDescription.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    m_AttachmentDescription.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    m_AttachmentDescription.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    m_AttachmentDescription.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    m_AttachmentDescription.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    if ((m_Usage & Usage::TextureAttachment) != 0u)
        m_AttachmentDescription.finalLayout = VK_IMAGE_LAYOUT_READ_ONLY_OPTIMAL;
    else if ((m_Usage & Usage::CopySource) != 0u)
        m_AttachmentDescription.finalLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
    else if ((m_Usage & Usage::DepthStencilAttachment) != 0u)
        m_AttachmentDescription.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
    else
        m_AttachmentDescription.finalLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

    CreateVulkanObjects();
}

void VulkanRenderTarget::CreateVulkanObjects() {
    auto context = VulkanContext::GetContextFromApplication();

    m_ImageCreateInfo.extent = { m_Width, m_Height, 1u };
    SB_VK_GRAPHICS_CALL_INFO(vkCreateImage(context->GetLogicalDevice(), &m_ImageCreateInfo, nullptr, &m_Image),
        "Failed to create Vulkan image");

    VkMemoryRequirements memoryRequirements;
    vkGetImageMemoryRequirements(context->GetLogicalDevice(), m_Image, &memoryRequirements);

    VkMemoryAllocateInfo allocateInfo{};
    allocateInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocateInfo.allocationSize = memoryRequirements.size;
    allocateInfo.memoryTypeIndex = context->FindMemoryType(memoryRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

    SB_VK_GRAPHICS_CALL_INFO(vkAllocateMemory(context->GetLogicalDevice(), &allocateInfo, nullptr, &m_ImageMemory),
        "Failed to allocate memory for Vulkan image");

    vkBindImageMemory(context->GetLogicalDevice(), m_Image, m_ImageMemory, 0u);

    m_ImageViewCreateInfo.image = m_Image;
    SB_VK_GRAPHICS_CALL_INFO(vkCreateImageView(context->GetLogicalDevice(), &m_ImageViewCreateInfo, nullptr, &m_ImageView),
        "Failed to create Vulkan image view");
}

void VulkanRenderTarget::DestroyVulkanObjects() {
    auto context = VulkanContext::GetContextFromApplication();

    vkDestroyImageView(context->GetLogicalDevice(), m_ImageView, nullptr);
    m_ImageView = nullptr;

    vkFreeMemory(context->GetLogicalDevice(), m_ImageMemory, nullptr);
    m_ImageMemory = nullptr;

    vkDestroyImage(context->GetLogicalDevice(), m_Image, nullptr);
    m_Image = nullptr;
}

void VulkanRenderTarget::Resize(uint32_t width, uint32_t height) {
    if ((m_Width == width && m_Height == height) || m_Width == 0u || m_Height == 0u)
        return;

    m_Width = width;
    m_Height = height;
    DestroyVulkanObjects();
    CreateVulkanObjects();
}

}
