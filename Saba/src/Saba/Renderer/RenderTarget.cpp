#include <pch.h>
#include "RenderTarget.h"

#include "Saba/Platform/Vulkan/VulkanRenderTarget.h"

namespace Saba {

Ref<RenderTarget> RenderTarget::Create(uint32_t width, uint32_t height, Format format, Usage usage, glm::vec4 clearVal) {
    return Ref<VulkanRenderTarget>::Create(width, height, format, usage, clearVal);
}

} //namespace Saba
