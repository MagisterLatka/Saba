#pragma once

#include "Saba/Core/Application.h"

#include <vulkan/vulkan.h>

namespace Saba {

class VulkanContext : public GraphicsContext {
public:
    VulkanContext() = default;
    ~VulkanContext() = default;

    void Init() override;
    void Shutdown() override;

    VkInstance GetInstance() const { return m_Instance; }
    VkPhysicalDevice GetPhysicalDevice() const { return m_PhysicalDevice; }
    VkDevice GetLogicalDevice() const { return m_Device; }
    uint32_t GetGraphicsFamilyIndex() const { return m_GraphicsQueueFamilyIndex; }
    VkQueue GetGrahicsQueue() const { return m_GraphicsQueue; }

    static Ref<VulkanContext> GetContextFromApplication() { return Application::Get().GetGraphicsContext().As<VulkanContext>(); }
private:
    void CreateInstance();
    void SetupDebugMessenger();
    void PickPhysicalDevice();
    void CreateLogicalDevice();
private:
    VkInstance m_Instance = nullptr;
    VkDebugUtilsMessengerEXT m_DebugMessenger = nullptr;

    VkPhysicalDevice m_PhysicalDevice = nullptr;
    VkDevice m_Device = nullptr;

    uint32_t m_GraphicsQueueFamilyIndex = std::numeric_limits<uint32_t>::max();
    VkQueue m_GraphicsQueue = nullptr;
};

#if defined(SB_DEBUG) || defined(SB_RELEASE)
#   define SB_VK_GRAPHICS_CALL_INFO(func, ...) { if ((func) != VK_SUCCESS)\
        { throw Saba::MessageException(__LINE__, __FILE__, std::format(__VA_ARGS__), true); } }
#else
#   define SB_VK_GRAPHICS_CALL_INFO(func, ...) { (func); }
#endif

} //namespace Saba
