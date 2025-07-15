#pragma once

#include "Saba/Renderer/GraphicsContext.h"
#include "Saba/Core/Application.h"
#include <vulkan/vulkan.h>

namespace Saba {

class VulkanContext : public GraphicsContext {
public:
    enum QueueType : uint8_t {
        GraphicsQueue, TransferQueue
    };
public:
    VulkanContext();
    ~VulkanContext();

    VkInstance GetInstance() const { return m_Instance; }
    VkPhysicalDevice GetPhysicalDevice() const { return m_PhysicalDevice; }
    VkDevice GetLogicalDevice() const { return m_Device; }

    template<QueueType Type>
    VkQueue GetQueue() const {
        if constexpr(Type == GraphicsQueue)
            return m_GraphicsQueue;
        else
            return m_TransferQueue;
    }
    template<QueueType Type>
    VkCommandPool GetCommandPool() const {
        if constexpr(Type == GraphicsQueue)
            return m_GraphicsCommandPool;
        else
            return m_TransferCommandPool;
    }

    std::optional<uint32_t> FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties) const;
    VkPhysicalDeviceProperties GetDeviceProperties() const;
    
    static Ref<VulkanContext> GetContextFromApplication() { return Application::Get().GetGraphicsContext().As<VulkanContext>(); }
private:
    void CreateInstance();
    void PickPhysicalDevice();
    void CreateLogicalDevice();
    void CreateCommandPools();

    static bool CheckValidationLayerSupport();

    using DeviceScore = std::multimap<uint32_t, VkPhysicalDevice>;
    static DeviceScore RatePhysicalDevices(const std::vector<VkPhysicalDevice>& physicalDevices);
private:
    VkInstance m_Instance = nullptr;
    VkDebugUtilsMessengerEXT m_DebugMessenger = nullptr;
    VkPhysicalDevice m_PhysicalDevice = nullptr;
    VkDevice m_Device = nullptr;

    uint32_t m_GraphicsQueueFamily = std::numeric_limits<uint32_t>::max();
    VkQueue m_GraphicsQueue = nullptr, m_TransferQueue = nullptr;
    VkCommandPool m_GraphicsCommandPool = nullptr, m_TransferCommandPool = nullptr;
};

#if defined(SB_DEBUG) || defined(SB_RELEASE)
#   define SB_VK_CALL(func,...) { if ((func) != VK_SUCCESS) { SB_CORE_THROW_INFO(__VA_ARGS__); } }
#else
#   define SB_VK_CALL(func, ...) (func)
#endif

} //namespace Saba
