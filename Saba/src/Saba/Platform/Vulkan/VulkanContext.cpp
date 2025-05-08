#include <pch.h>
#include "VulkanContext.h"

namespace Saba {

#if defined(SB_DEBUG) || defined(SB_RELEASE)
    static constexpr bool c_EnableValidationLayers = true;
#else
    static constexpr bool c_EnableValidationLayers = false;
#endif

static constexpr auto c_ValidationLayers = std::to_array<const char*>({
    "VK_LAYER_KHRONOS_validation"
});
static bool CheckValidationLayerSupport() {
    uint32_t layerCount;
    vkEnumerateInstanceLayerProperties(&layerCount, nullptr);
    std::vector<VkLayerProperties> instanceLayers(layerCount);
    vkEnumerateInstanceLayerProperties(&layerCount, instanceLayers.data());

    for (const auto& layerName : c_ValidationLayers) {
        if (std::ranges::none_of(instanceLayers, [&layerName](const VkLayerProperties& elem) { return std::string(elem.layerName) == layerName; } ))
            return false;
    }

    return true;
}

static constexpr auto c_DeviceExtensions = std::to_array<const char*>({
    VK_KHR_SWAPCHAIN_EXTENSION_NAME
});
static bool CheckDeviceExtensionSupport(VkPhysicalDevice device) {
    uint32_t extensionCount = 0u;
    vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);
    std::vector<VkExtensionProperties> availableExtensions(extensionCount);
    vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

    std::set<std::string> requiredExtensions(c_DeviceExtensions.begin(), c_DeviceExtensions.end());
    for (const auto& extension : availableExtensions) {
        requiredExtensions.erase(extension.extensionName);
    }
    return requiredExtensions.empty();
}

std::optional<uint32_t> FindGraphicsQueueFamily(VkPhysicalDevice device) {
    uint32_t queueFamilyCount = 0u;
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);
    std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

    for (const auto& [i, family] : queueFamilies | std::views::enumerate) {
        if ((family.queueFlags & VK_QUEUE_GRAPHICS_BIT) != 0)
            return static_cast<uint32_t>(i);
    }
    return {};
}
std::pair<bool, std::optional<uint32_t>> IsDeviceSuitable(VkPhysicalDevice device) {
    auto graphicsQueuFamilyIndex = FindGraphicsQueueFamily(device);
    bool extensionsSupported = CheckDeviceExtensionSupport(device);
    return { graphicsQueuFamilyIndex.has_value() && extensionsSupported, graphicsQueuFamilyIndex };
}
uint32_t RankDeviceSuitability(VkPhysicalDevice device) {
    uint32_t score = 0u;

    VkPhysicalDeviceProperties deviceProperties;
    vkGetPhysicalDeviceProperties(device, &deviceProperties);
    VkPhysicalDeviceFeatures deviceFeatures;
    vkGetPhysicalDeviceFeatures(device, &deviceFeatures);

    switch (deviceProperties.deviceType) {
        case VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU:
            score += 0x20000;
            break;
        case VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU:
            score += 0x10000;
            break;
        default:
            break;
    }

    score += deviceProperties.limits.maxImageDimension2D;

    return score;
}

static VKAPI_ATTR VkBool32 VKAPI_CALL DebugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT severity,
    VkDebugUtilsMessageTypeFlagsEXT type, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, [[maybe_unused]] void* pUserData)
{
    std::string messageType;
    if ((type & VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT) != 0)
        messageType += "General, ";
    if ((type & VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT) != 0)
        messageType += "Validation, ";
    if ((type & VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT) != 0)
        messageType += "Performance, ";

    if (!messageType.empty())
        messageType.erase(messageType.length() - 2);

    if (severity >= VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT) {
        SB_CORE_CRITICAL("Vulkan debug error ({0}): {1}", messageType, pCallbackData->pMessage);
        return VK_FALSE;
    }
    if (severity >= VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT) {
        SB_CORE_WARN("Vulkan debug warning ({0}): {1}", messageType, pCallbackData->pMessage);
        return VK_FALSE;
    }
    return VK_FALSE;
}
VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
    const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger)
{
    auto func = reinterpret_cast<PFN_vkCreateDebugUtilsMessengerEXT>(vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT"));
    if (func != nullptr)
        return func(instance, pCreateInfo, pAllocator, pDebugMessenger);

    return VK_ERROR_EXTENSION_NOT_PRESENT;
}
void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator) {
    auto func = reinterpret_cast<PFN_vkDestroyDebugUtilsMessengerEXT>(vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT"));
    if (func != nullptr)
        func(instance, debugMessenger, pAllocator);
}
static void PopulateDebugMessengerData(VkDebugUtilsMessengerCreateInfoEXT& createInfo) {
    createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
    createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
    createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT
        | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
    createInfo.pfnUserCallback = DebugCallback;
}

VulkanContext::VulkanContext() {
    Init();
}
VulkanContext::~VulkanContext() {
    vkDestroyDevice(m_Device, nullptr);

    if constexpr (c_EnableValidationLayers)
        DestroyDebugUtilsMessengerEXT(m_Instance, m_DebugMessenger, nullptr);

    vkDestroyInstance(m_Instance, nullptr);
}

void VulkanContext::Init() {
    CreateInstance();
    SetupDebugMessenger();
    PickPhysicalDevice();
    CreateLogicalDevice();
}
void VulkanContext::CreateInstance() {
    SB_CORE_ASSERT(!c_EnableValidationLayers || CheckValidationLayerSupport(),
        "Requested Vulkan validation layers not available");

    VkApplicationInfo appInfo{};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pEngineName = "Saba Engine";
    appInfo.apiVersion = VK_API_VERSION_1_0;

    VkInstanceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &appInfo;

    std::vector<const char*> extensions = {
        VK_KHR_SURFACE_EXTENSION_NAME
#if defined(SB_PLATFORM_LINUX)
        "VK_KHR_xcb_surface"
#elif defined(SB_PLATFORM_WINDOWS)
        "VK_KHR_win32_surface"
#else
#   error
#endif
    };
    if constexpr (c_EnableValidationLayers)
        extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);

    createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
    createInfo.ppEnabledExtensionNames = extensions.data();

    VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo{};
    if constexpr (c_EnableValidationLayers) {
        createInfo.enabledLayerCount = static_cast<uint32_t>(c_ValidationLayers.size());
        createInfo.ppEnabledLayerNames = c_ValidationLayers.data();
        
        PopulateDebugMessengerData(debugCreateInfo);
        createInfo.pNext = &debugCreateInfo;
    }

    SB_VK_GRAPHICS_CALL_INFO(vkCreateInstance(&createInfo, nullptr, &m_Instance), "Failed to create vulkan instance");
}
void VulkanContext::SetupDebugMessenger() {
    if constexpr (!c_EnableValidationLayers)
        return;

    VkDebugUtilsMessengerCreateInfoEXT createInfo{};
    PopulateDebugMessengerData(createInfo);

    SB_VK_GRAPHICS_CALL_INFO(CreateDebugUtilsMessengerEXT(m_Instance, &createInfo, nullptr, &m_DebugMessenger),
        "Failed to set up Vulkan debug messenger");
}
void VulkanContext::PickPhysicalDevice() {
    uint32_t deviceCount = 0u;
    vkEnumeratePhysicalDevices(m_Instance, &deviceCount, nullptr);
    SB_CORE_ASSERT(deviceCount != 0u, "Failed to find GPUs with Vulkan support");

    std::vector<VkPhysicalDevice> devices(deviceCount);
    vkEnumeratePhysicalDevices(m_Instance, &deviceCount, devices.data()); 

    std::multimap<int, std::pair<VkPhysicalDevice, uint32_t>> deviceRank;
    for (const auto& device : devices) {
        auto [isSuitable, graphicsQueueFamilyIndex] = IsDeviceSuitable(device);
        if (isSuitable) {
            uint32_t score = RankDeviceSuitability(device);
            if (score != 0u)
                deviceRank.insert(std::make_pair(score, std::make_pair(device, *graphicsQueueFamilyIndex)));
        }
    }
    SB_CORE_ASSERT(!deviceRank.empty(), "Failed to find a suitable GPU");
    std::tie(m_PhysicalDevice, m_GraphicsQueueFamilyIndex) = deviceRank.rbegin()->second;
}
void VulkanContext::CreateLogicalDevice() {
    VkDeviceQueueCreateInfo queueCreateInfo;
    auto queuePriorities = std::to_array<float>({ 0.9f, 1.0f });
    queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queueCreateInfo.queueFamilyIndex = m_GraphicsQueueFamilyIndex;
    queueCreateInfo.queueCount = static_cast<uint32_t>(queuePriorities.size());
    queueCreateInfo.pQueuePriorities = queuePriorities.data();

    VkPhysicalDeviceFeatures deviceFeatures{};

    VkDeviceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    createInfo.pQueueCreateInfos = &queueCreateInfo;
    createInfo.queueCreateInfoCount = 1u;
    createInfo.pEnabledFeatures = &deviceFeatures;
    createInfo.enabledExtensionCount = static_cast<uint32_t>(c_DeviceExtensions.size());
    createInfo.ppEnabledExtensionNames = c_DeviceExtensions.data();

    if constexpr (c_EnableValidationLayers) {
        createInfo.enabledLayerCount = static_cast<uint32_t>(c_ValidationLayers.size());
        createInfo.ppEnabledLayerNames = c_ValidationLayers.data();
    }

    SB_VK_GRAPHICS_CALL_INFO(vkCreateDevice(m_PhysicalDevice, &createInfo, nullptr, &m_Device),
        "Failed to create Vulkan logical device");

    vkGetDeviceQueue(m_Device, m_GraphicsQueueFamilyIndex, 0u, &m_GraphicsQueue);
    vkGetDeviceQueue(m_Device, m_GraphicsQueueFamilyIndex, 1u, &m_TransferQueue);
}

uint32_t VulkanContext::FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties) {
    VkPhysicalDeviceMemoryProperties memProperties;
    vkGetPhysicalDeviceMemoryProperties(m_PhysicalDevice, &memProperties);

    for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
        if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties) {
            return i;
        }
    }
    SB_CORE_THROW_INFO("Failed to find suitable Vulkan memory type");
}

}
