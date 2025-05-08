#include <pch.h>
#include "RenderPass.h"

#include "Saba/Platform/Vulkan/VulkanRenderPass.h"

namespace Saba {

Ref<RenderPass> RenderPass::Create(const std::vector<Ref<RenderSubpass>>& subpasses) {
    return Ref<VulkanRenderPass>::Create(subpasses);
}

} //namespace Saba
