#include <pch.h>
#include "GraphicsContext.h"

#include "Saba/Platform/Vulkan/VulkanContext.h"

namespace Saba {

Ref<GraphicsContext> GraphicsContext::Create() {
    return Ref<VulkanContext>::Create();
}

} //namespace Saba
