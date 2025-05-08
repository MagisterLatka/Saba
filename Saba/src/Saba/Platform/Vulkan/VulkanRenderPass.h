#pragma once

#include "Saba/Renderer/RenderPass.h"
#include "Saba/Platform/Vulkan/VulkanRenderTarget.h"

#include <vulkan/vulkan.h>

namespace Saba {

class VulkanRenderPass : public RenderPass {
public:
    VulkanRenderPass(const std::vector<Ref<RenderSubpass>>& subpasses);
    ~VulkanRenderPass();

    Ref<RenderTarget> GetRenderTarget(uint32_t index = 0) const override {
        SB_CORE_ASSERT(index < 8, "Exceeding available number of simultaneous render targets");
        return m_RenderTargets[index];
    }
    Ref<RenderTarget> GetDepthStencilTarget() const noexcept override { return m_DepthStencilTarget; }
private:
    void Init(const std::vector<Ref<RenderSubpass>>& subpasses);
private:
    std::vector<Ref<VulkanRenderTarget>> m_RenderTargets;
    Ref<VulkanRenderTarget> m_DepthStencilTarget;

    VkRenderPass m_RenderPass = nullptr;

    uint32_t m_Width = 0u, m_Height = 0u;
};

}
