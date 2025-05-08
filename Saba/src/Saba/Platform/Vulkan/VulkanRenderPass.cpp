#include <pch.h>
#include "VulkanRenderPass.h"

#include "Saba/Platform/Vulkan/VulkanContext.h"

namespace Saba {

VulkanRenderPass::VulkanRenderPass() {
    Init();
}
VulkanRenderPass::~VulkanRenderPass() {
    auto context = VulkanContext::GetContextFromApplication();
    vkDestroyRenderPass(context->GetLogicalDevice(), m_RenderPass, nullptr);
}
void VulkanRenderPass::Init() {
    auto context = VulkanContext::GetContextFromApplication();

    VkRenderPassCreateInfo renderPassInfo{};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;

    SB_VK_GRAPHICS_CALL_INFO(vkCreateRenderPass(context->GetLogicalDevice(), &renderPassInfo, nullptr, &m_RenderPass),
        "Failed to create Vulkan render pass");
}

}