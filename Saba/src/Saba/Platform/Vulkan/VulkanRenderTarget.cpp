#include <pch.h>
#include "VulkanRenderTarget.h"

#include "Saba/Platform/Vulkan/VulkanContext.h"

namespace Saba {

static VkFormat GetVulkanFormat(RenderTarget::Format format);
static VkImageUsageFlags GetVulkanUsage(RenderTarget::Usage usage);

VulkanRenderTarget::VulkanRenderTarget(uint32_t width, uint32_t height, Format format, Usage usage, ClearValue clearVal)
    : m_Width(width), m_Height(height), m_Format(format), m_Usage(usage), m_ClearValue(clearVal)
{
    auto properties = VulkanContext::GetContextFromApplication()->GetDeviceProperties();

    SB_CORE_ASSERT(width != 0 && width <= properties.limits.maxFramebufferWidth
        && height != 0 && height <= properties.limits.maxFramebufferHeight,
        "Width and height of a render target must fit within valid range "
        "supported by the device: width (0, {0}], height (0, {1}]",
        properties.limits.maxFramebufferWidth, properties.limits.maxFramebufferHeight);

    CreateImage();
    CreateImageView();
}
VulkanRenderTarget::~VulkanRenderTarget() {
    Destroy();
}

void VulkanRenderTarget::Resize(uint32_t width, uint32_t height) {
    Destroy();

    m_Width = width;
    m_Height = height;

    CreateImage();
    CreateImageView();
}

void VulkanRenderTarget::CreateImage() {
    VkImageCreateInfo createInfo{
        .sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO,
        .pNext = nullptr,
        .flags = 0u,
        .imageType = VK_IMAGE_TYPE_2D,
        .format = GetVulkanFormat(m_Format),
        .extent = VkExtent3D(m_Width, m_Height, 1u),
        .mipLevels = 1u,
        .arrayLayers = 1u,
        .samples = VK_SAMPLE_COUNT_1_BIT,
        .tiling = VK_IMAGE_TILING_OPTIMAL,
        .usage = GetVulkanUsage(m_Usage),
        .sharingMode = VK_SHARING_MODE_EXCLUSIVE,
        .queueFamilyIndexCount = 0u,
        .pQueueFamilyIndices = nullptr,
        .initialLayout = VK_IMAGE_LAYOUT_UNDEFINED
    };

    if (m_Format == Format::Depth32F || m_Format == Format::Depth24Stencil8)
        createInfo.usage |= VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
    else
        createInfo.usage |= VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

    auto context = VulkanContext::GetContextFromApplication();
    SB_VK_CALL(vkCreateImage(context->GetLogicalDevice(), &createInfo, nullptr, &m_Image),
        "Failed to create render target Vulkan image");

    VkMemoryRequirements memRequirements;
    vkGetImageMemoryRequirements(context->GetLogicalDevice(), m_Image, &memRequirements);

    auto memoryType = context->FindMemoryType(memRequirements.memoryTypeBits,
        VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
    SB_CORE_ASSERT(memoryType.has_value(), "Failed to find suitable memory type for render target"
        "Vulkan image");
    VkMemoryAllocateInfo allocateInfo{
        .sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
        .pNext = nullptr,
        .allocationSize = memRequirements.size,
        .memoryTypeIndex = *memoryType
    };

    SB_VK_CALL(vkAllocateMemory(context->GetLogicalDevice(), &allocateInfo, nullptr, &m_ImageMemory),
        "Failed to allocate memory for render target Vulkan image");

    vkBindImageMemory(context->GetLogicalDevice(), m_Image, m_ImageMemory, 0u);
}

void VulkanRenderTarget::CreateImageView() {
    VkImageViewCreateInfo createInfo{
        .sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
        .pNext = nullptr,
        .flags = 0u,
        .image = m_Image,
        .viewType = VK_IMAGE_VIEW_TYPE_2D,
        .format = GetVulkanFormat(m_Format),
        .components = {
            .r = VK_COMPONENT_SWIZZLE_IDENTITY, .g = VK_COMPONENT_SWIZZLE_IDENTITY,
            .b = VK_COMPONENT_SWIZZLE_IDENTITY, .a = VK_COMPONENT_SWIZZLE_IDENTITY
        },
        .subresourceRange = {
            .aspectMask = 0u,
            .baseMipLevel = 0u,
            .levelCount = 1u,
            .baseArrayLayer = 0u,
            .layerCount = 1u
        }
    };

    if (m_Format == Format::Depth32F)
        createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;
    else if (m_Format == Format::Depth24Stencil8)
        createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT | VK_IMAGE_ASPECT_STENCIL_BIT;
    else
        createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;

    auto context = VulkanContext::GetContextFromApplication();
    SB_VK_CALL(vkCreateImageView(context->GetLogicalDevice(), &createInfo, nullptr, &m_ImageView),
        "Failed to create render target Vulkan image view");
}

void VulkanRenderTarget::Destroy() {
    auto context = VulkanContext::GetContextFromApplication();

    vkDestroyImageView(context->GetLogicalDevice(), m_ImageView, nullptr);
    m_ImageView = nullptr;

    vkFreeMemory(context->GetLogicalDevice(), m_ImageMemory, nullptr);
    m_ImageMemory = nullptr;

    vkDestroyImage(context->GetLogicalDevice(), m_Image, nullptr);
    m_Image = nullptr;
}

static VkFormat GetVulkanFormat(RenderTarget::Format format) {
    switch (format) {
        case RenderTarget::Format::R8:
            return VK_FORMAT_R8_SNORM;
        case RenderTarget::Format::R32F:
            return VK_FORMAT_R32_SFLOAT;
        case RenderTarget::Format::R32_UINT:
            return VK_FORMAT_R32_UINT;
        case RenderTarget::Format::RG8:
            return VK_FORMAT_R8G8_SNORM;
        case RenderTarget::Format::RG16F:
            return VK_FORMAT_R16G16_SFLOAT;
        case RenderTarget::Format::RG32F:
            return VK_FORMAT_R32G32_SFLOAT;
        case RenderTarget::Format::RG32_UINT:
            return VK_FORMAT_R32G32_UINT;
        case RenderTarget::Format::RGB8:
            return VK_FORMAT_R8G8B8_SNORM;
        case RenderTarget::Format::RGB32F:
            return VK_FORMAT_R32G32B32_SFLOAT;
        case RenderTarget::Format::RGB32_UINT:
            return VK_FORMAT_R32G32B32_UINT;
        case RenderTarget::Format::RGBA8:
            return VK_FORMAT_R8G8B8A8_SNORM;
        case RenderTarget::Format::RGBA16F:
            return VK_FORMAT_R16G16B16A16_SFLOAT;
        case RenderTarget::Format::RGBA32F:
            return VK_FORMAT_R32G32B32A32_SFLOAT;
        case RenderTarget::Format::RGBA32_UINT:
            return VK_FORMAT_R32G32B32A32_UINT;
        case RenderTarget::Format::Depth32F:
            return VK_FORMAT_D32_SFLOAT;
        case RenderTarget::Format::Depth24Stencil8:
            return VK_FORMAT_D24_UNORM_S8_UINT;
    }
    SB_CORE_THROW_INFO("Unknown render target format");
}
static VkImageUsageFlags GetVulkanUsage(RenderTarget::Usage usage) {
    VkImageUsageFlags flags = 0u;
    if ((usage & RenderTarget::TransferSourceUsage) == 0u)
        flags |= VK_IMAGE_USAGE_TRANSFER_SRC_BIT;
    if ((usage & RenderTarget::TransferDestinationUsage) == 0u)
        flags |= VK_IMAGE_USAGE_TRANSFER_DST_BIT;
    if ((usage & RenderTarget::Usage::TextureUsage) == 0u)
        flags |= VK_IMAGE_USAGE_SAMPLED_BIT;
    if ((usage & RenderTarget::Usage::InputAttachmentUsage) == 0u)
        flags |= VK_IMAGE_USAGE_INPUT_ATTACHMENT_BIT;
    SB_CORE_ASSERT(!(flags == 0u && usage != 0u), "Unknown render target usage");
    return flags;
}

}
