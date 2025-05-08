#include <pch.h>
#include "VulkanRenderPass.h"

#include "Saba/Platform/Vulkan/VulkanContext.h"

namespace Saba {

VulkanRenderPass::VulkanRenderPass(const std::vector<Ref<RenderSubpass>>& subpasses) {
    SB_CORE_ASSERT(!subpasses.empty(), "Render pass requires at least one subpass");
    SB_CORE_ASSERT(subpasses.size() <= 8, "Render pass requires at most 8 subpasses");

    for (const auto& subpass : subpasses) {
        for (const auto& renderTarget : subpass->GetRenderTargets() | std::views::filter([](const auto& elem) { return elem.HasValue(); })
            | std::views::transform([](const auto& elem) { return elem.template As<VulkanRenderTarget>(); }))
        {
            if (std::ranges::find(m_RenderTargets, renderTarget) == m_RenderTargets.end())
                m_RenderTargets.push_back(renderTarget);
        }
        for (const auto& renderTarget : subpass->GetInputTargets() | std::views::filter([](const auto& elem) { return elem.HasValue(); })
            | std::views::transform([](const auto& elem) { return elem.template As<VulkanRenderTarget>(); }))
        {
            if (std::ranges::find(m_RenderTargets, renderTarget) == m_RenderTargets.end())
                m_RenderTargets.push_back(renderTarget);
        }
        if (!m_DepthStencilTarget.HasValue())
            m_DepthStencilTarget = subpass->GetDepthStencilTarget().As<VulkanRenderTarget>();
        else {
            SB_CORE_ASSERT(m_DepthStencilTarget == subpass->GetDepthStencilTarget().As<VulkanRenderTarget>(),
                "Render pass might have only one depth stencil attachment");
        }
    }

    SB_CORE_ASSERT(m_RenderTargets.size() <= 8, "Only 8 render targets might be used in render pass");
    SB_CORE_ASSERT(m_RenderTargets.size() > 0 || m_DepthStencilTarget.HasValue(),
        "Render pass requires at least one render target or a depth stencil render target");

    for (const auto& renderTarget : m_RenderTargets) {
        if (m_Width == 0u && m_Height == 0u) {
            m_Width = renderTarget->GetWidth();
            m_Height = renderTarget->GetHeight();
        }
        else {
            SB_CORE_ASSERT(m_Width == renderTarget->GetWidth() && m_Height == renderTarget->GetHeight(),
                "Render pass requires all its render targets to have the same width and height");
        }
    }
    if (m_Width == 0u && m_Height == 0u) {
        m_Width = m_DepthStencilTarget->GetWidth();
        m_Height = m_DepthStencilTarget->GetHeight();
    }
    else {
        SB_CORE_ASSERT(m_Width == m_DepthStencilTarget->GetWidth() && m_Height == m_DepthStencilTarget->GetHeight(),
            "Render pass requires its depth stencil render target to have the same width and height");
    }

    Init(subpasses);
}
VulkanRenderPass::~VulkanRenderPass() {
    auto context = VulkanContext::GetContextFromApplication();
    vkDestroyRenderPass(context->GetLogicalDevice(), m_RenderPass, nullptr);
}
void VulkanRenderPass::Init(const std::vector<Ref<RenderSubpass>>& subpasses) {
    auto context = VulkanContext::GetContextFromApplication();

    std::vector<VkAttachmentDescription> attachmentDescriptions;
    attachmentDescriptions.reserve(m_RenderTargets.size() + (m_DepthStencilTarget.HasValue() ? 1u : 0u));
    for (auto renderTarget : m_RenderTargets)
        attachmentDescriptions.push_back(renderTarget->GetAttachmentDescription());
    if (m_DepthStencilTarget)
        attachmentDescriptions.push_back(m_DepthStencilTarget->GetAttachmentDescription());

    std::vector<std::vector<VkAttachmentReference>> attachmentReferences;
    std::vector<std::vector<uint32_t>> preserveAttachments;
    std::vector<bool> useDepthStencilAttachment;
    for (const auto& subpass : subpasses) {
        std::set<uint32_t> unusedAttachments(std::views::iota(0u, m_RenderTargets.size()));
        for (const auto& renderTarget : subpass->GetRenderTargets()) {

        }
    }

    VkRenderPassCreateInfo renderPassInfo{};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;

    SB_VK_GRAPHICS_CALL_INFO(vkCreateRenderPass(context->GetLogicalDevice(), &renderPassInfo, nullptr, &m_RenderPass),
        "Failed to create Vulkan render pass");
}

}
