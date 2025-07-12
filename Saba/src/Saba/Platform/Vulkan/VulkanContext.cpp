#include <pch.h>
#include "VulkanContext.h"

namespace Saba {

static constexpr uint32_t c_RequiredVulkanVersion = VK_API_VERSION_1_0;

#if defined(SB_DEBUG) || defined(SB_RELEASE)
    static constexpr bool c_EnableValidationLayers = true;
#else
    static constexpr bool c_EnableValidationLayers = false;
#endif

static constexpr auto c_ValidationLayers = std::to_array<const char*>({
    "VK_LAYER_KHRONOS_validation"
});
static constexpr auto c_RequiredExtensions = std::to_array<const char*>({
    VK_KHR_SURFACE_EXTENSION_NAME,

#if defined(SB_PLATFORM_LINUX)
    "VK_KHR_xlib_surface",
#elif defined(SB_PLATFORM_WINDOWS)
    "VK_KHR_win32_surface",
#endif

#if defined(SB_DEBUG) || defined(SB_RELEASE)
    VK_EXT_DEBUG_UTILS_EXTENSION_NAME,
#endif
});
static constexpr auto c_DeviceExtensions = std::to_array<const char*>({
    VK_KHR_SWAPCHAIN_EXTENSION_NAME
});

static VKAPI_ATTR VkBool32 VKAPI_CALL VulkanDebugCallback(
    VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType,
    const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, [[maybe_unused]] void* pUserData);
static VkResult CreateDebugUtilsMessengerEXT(VkInstance instance,
    const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator,
    VkDebugUtilsMessengerEXT* pDebugMessenger);
static void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger,
    const VkAllocationCallbacks* pAllocator);

VulkanContext::VulkanContext() {
    CreateInstance();
    PickPhysicalDevice();
    CreateLogicalDevice();
    CreateCommandPools();
}

VulkanContext::~VulkanContext() {
    vkDeviceWaitIdle(m_Device);
    vkDestroyCommandPool(m_Device, m_GraphicsCommandPool, nullptr);
    vkDestroyCommandPool(m_Device, m_TransferCommandPool, nullptr);
    vkDestroyDevice(m_Device, nullptr);

    if constexpr(c_EnableValidationLayers) {
        DestroyDebugUtilsMessengerEXT(m_Instance, m_DebugMessenger, nullptr);
    }
    vkDestroyInstance(m_Instance, nullptr);
}

void VulkanContext::CreateInstance() {
    SB_CORE_ASSERT(!c_EnableValidationLayers || CheckValidationLayerSupport(),
        "Vulkan validation layers not available");

    VkApplicationInfo appInfo{
        .sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
        .pNext = nullptr,
        .pApplicationName = "Saba Engine",
        .applicationVersion = VK_MAKE_VERSION(0, 0, 1),
        .pEngineName = "Saba Engine",
        .engineVersion = VK_MAKE_VERSION(0, 0, 1),
        .apiVersion = VK_API_VERSION_1_0
    };

    VkInstanceCreateInfo createInfo{
        .sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
        .pNext = nullptr,
        .flags = 0u,
        .pApplicationInfo = &appInfo,
        .enabledLayerCount = 0u,
        .ppEnabledLayerNames = nullptr,
        .enabledExtensionCount = c_RequiredExtensions.size(),
        .ppEnabledExtensionNames = c_RequiredExtensions.data()
    };

    VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo{
        .sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT,
        .pNext = nullptr,
        .flags = 0u,
        .messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT
            | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT,
        .messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT
            | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT
            | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT,
        .pfnUserCallback = VulkanDebugCallback,
        .pUserData = nullptr
    };
    if constexpr (c_EnableValidationLayers) {
        createInfo.enabledLayerCount = c_ValidationLayers.size();
        createInfo.ppEnabledLayerNames = c_ValidationLayers.data();
        createInfo.pNext = &debugCreateInfo;
    }

    SB_VK_CALL(vkCreateInstance(&createInfo, nullptr, &m_Instance),
        "Failed to create Vulkan instance");

    if constexpr (c_EnableValidationLayers) {
        SB_VK_CALL(CreateDebugUtilsMessengerEXT(m_Instance, &debugCreateInfo, nullptr, &m_DebugMessenger),
            "Failed to create Vulkan debug messenger");
    }
}

void VulkanContext::PickPhysicalDevice() {
    uint32_t deviceCount = 0u;
    vkEnumeratePhysicalDevices(m_Instance, &deviceCount, nullptr);
    SB_CORE_ASSERT(deviceCount != 0u, "Failed to find any GPU with Vulkan support");

    std::vector<VkPhysicalDevice> devices(deviceCount);
    vkEnumeratePhysicalDevices(m_Instance, &deviceCount, devices.data());

    auto deviceScores = RatePhysicalDevices(devices);
    SB_CORE_ASSERT(!deviceScores.empty() && deviceScores.rbegin()->first > 0u,
        "Failed to find a suitable GPU");
    m_PhysicalDevice = deviceScores.rbegin()->second;
}

void VulkanContext::CreateLogicalDevice() {
    uint32_t queueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(m_PhysicalDevice, &queueFamilyCount, nullptr);

    std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(m_PhysicalDevice, &queueFamilyCount, queueFamilies.data());

    for (const auto& [index, queueFamily] : queueFamilies | std::views::enumerate) {
        if ((queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) != 0u) {
            m_GraphicsQueueFamily = static_cast<uint32_t>(index);
            break;
        }
    }

    constexpr auto queuePriorities = std::to_array({ 0.9f, 1.0f });
    VkDeviceQueueCreateInfo queueCreateInfo{};
    queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queueCreateInfo.queueFamilyIndex = m_GraphicsQueueFamily;
    queueCreateInfo.queueCount = queuePriorities.size();
    queueCreateInfo.pQueuePriorities = queuePriorities.data();

    VkPhysicalDeviceFeatures deviceFeatures{};

    VkDeviceCreateInfo createInfo{
        .sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
        .pNext = nullptr,
        .flags = 0u,
        .queueCreateInfoCount = 1u,
        .pQueueCreateInfos = &queueCreateInfo,
        .enabledLayerCount = 0u,
        .ppEnabledLayerNames = nullptr,
        .enabledExtensionCount = 0u,
        .ppEnabledExtensionNames = nullptr,
        .pEnabledFeatures = &deviceFeatures
    };

    SB_VK_CALL(vkCreateDevice(m_PhysicalDevice, &createInfo, nullptr, &m_Device),
        "Failed to create Vulkan logical device");

    vkGetDeviceQueue(m_Device, m_GraphicsQueueFamily, 0u, &m_GraphicsQueue);
    vkGetDeviceQueue(m_Device, m_GraphicsQueueFamily, 0u, &m_TransferQueue);
}

void VulkanContext::CreateCommandPools() {
    VkCommandPoolCreateInfo poolInfo{
        .sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
        .pNext = nullptr,
        .flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT,
        .queueFamilyIndex = m_GraphicsQueueFamily
    };
    SB_VK_CALL(vkCreateCommandPool(m_Device, &poolInfo, nullptr, &m_GraphicsCommandPool),
        "Failed to create Vulkan graphics command pool");

    poolInfo.flags = poolInfo.flags | VK_COMMAND_POOL_CREATE_TRANSIENT_BIT;
    SB_VK_CALL(vkCreateCommandPool(m_Device, &poolInfo, nullptr, &m_GraphicsCommandPool),
        "Failed to create Vulkan transfer command pool");
}

bool VulkanContext::CheckValidationLayerSupport() {
    uint32_t layerCount;
    vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

    std::vector<VkLayerProperties> availableLayers(layerCount);
    vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

    for (const auto& layer : c_ValidationLayers) {
        if (std::ranges::none_of(availableLayers, [layer](const auto& elem) {
            return std::strcmp(elem.layerName, layer) == 0;
        })) return false;
    }
    return true;
}

VulkanContext::DeviceScore VulkanContext::RatePhysicalDevices(const std::vector<VkPhysicalDevice>& physicalDevices) {
    DeviceScore devices;
    for (const auto& device : physicalDevices) {
        VkPhysicalDeviceProperties deviceProperties;
        VkPhysicalDeviceFeatures deviceFeatures;
        vkGetPhysicalDeviceProperties(device, &deviceProperties);
        vkGetPhysicalDeviceFeatures(device, &deviceFeatures);

        uint32_t extensionCount;
        vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);
        std::vector<VkExtensionProperties> availableExtensions(extensionCount);
        vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

        std::set<std::string> requiredExtensions(std::from_range, c_DeviceExtensions);
        for (const auto& extension : availableExtensions) {
            requiredExtensions.erase(extension.extensionName);
        }

        uint32_t queueFamilyCount = 0;
        vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

        std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
        vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

        if (deviceProperties.apiVersion < c_RequiredVulkanVersion
            || deviceProperties.limits.maxColorAttachments < 8
            || !requiredExtensions.empty()
            || std::ranges::none_of(queueFamilies, [](const auto& elem)
                { return elem.queueFlags & VK_QUEUE_GRAPHICS_BIT; }))
        {
            devices.insert(std::make_pair(0, device));
            break;
        }

        uint32_t score = 0u;
        if (deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
            score += 10000;
        else if (deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU)
            score += 1000;

        score += deviceProperties.limits.maxImageDimension2D;
        score += deviceProperties.limits.maxPushConstantsSize * 10;
        score += deviceProperties.limits.maxMemoryAllocationCount * 10;
        score += deviceProperties.limits.maxBoundDescriptorSets * 10;

        devices.insert(std::make_pair(score, device));
    }
    return devices;
}

static VKAPI_ATTR VkBool32 VKAPI_CALL VulkanDebugCallback(
    VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType,
    const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, [[maybe_unused]] void* pUserData)
{
    std::string type;
    if ((messageType & VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT) != 0u)
        type += "general, ";
    if ((messageType & VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT) != 0u)
        type += "validation, ";
    if ((messageType & VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT) != 0u)
        type += "performance, ";
    if (!type.empty())
        type.erase(type.size() - 2);

    switch (messageSeverity) {
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT:
            SB_CORE_TRACE("Vulkan {0} message: {1}", type, pCallbackData->pMessage);
            break;
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT:
            SB_CORE_INFO("Vulkan {0} info: {1}", type, pCallbackData->pMessage);
            break;
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:
            SB_CORE_WARN("Vulkan {0} warning: {1}", type, pCallbackData->pMessage);
            break;
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:
        default:
            SB_CORE_ERROR("Vulkan {0} error: {1}", type, pCallbackData->pMessage);
            break;
    }
    return VK_FALSE;
}

static VkResult CreateDebugUtilsMessengerEXT(VkInstance instance,
    const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator,
    VkDebugUtilsMessengerEXT* pDebugMessenger)
{
    auto func = reinterpret_cast<PFN_vkCreateDebugUtilsMessengerEXT>(
        vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT"));

    if (func != nullptr) return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
    return VK_ERROR_EXTENSION_NOT_PRESENT;
}
static void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger,
    const VkAllocationCallbacks* pAllocator)
{
    auto func = reinterpret_cast<PFN_vkDestroyDebugUtilsMessengerEXT>(
        vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT"));
    if (func != nullptr) func(instance, debugMessenger, pAllocator);
}

}
