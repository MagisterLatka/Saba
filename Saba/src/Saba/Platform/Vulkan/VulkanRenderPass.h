#pragma once

#include "Saba/Core/Core.h"

#include <vulkan/vulkan.h>

namespace Saba {

class VulkanRenderPass : public RefCounted {
public:
    VulkanRenderPass();
    ~VulkanRenderPass();
private:
    void Init();
private:
    VkRenderPass m_RenderPass;
};

}