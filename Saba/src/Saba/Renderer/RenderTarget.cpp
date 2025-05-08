#include <pch.h>
#include "RenderTarget.h"

#include "Saba/Platform/Vulkan/VulkanRenderTarget.h"

namespace Saba {

Ref<RenderTarget> RenderTarget::Create(uint32_t width, uint32_t height, RenderTargetFormat format, RenderTargetUsage additionalUsage, glm::vec4 clearVal) {
    return Ref<VulkanRenderTarget>::Create(width, height, format, additionalUsage, clearVal);
}
Ref<RenderTarget> RenderTarget::Create(uint32_t width, uint32_t height, RenderTargetFormat format, RenderTargetUsage additionalUsage,
    float depthClearVal, uint8_t stencilClearVal)
{
    return Ref<VulkanRenderTarget>::Create(width, height, format, additionalUsage, depthClearVal, stencilClearVal);
}

} //namespace Saba
