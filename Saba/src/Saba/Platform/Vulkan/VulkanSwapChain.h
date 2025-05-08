#pragma once

#include <vulkan/vulkan.h>

namespace Saba {

class VulkanSwapChain {
public:
    VulkanSwapChain(VkSurfaceKHR surface, uint32_t width, uint32_t height);
    ~VulkanSwapChain();

    void Resize(uint32_t width, uint32_t height);
private:
    void Init();

    struct SwapChainSupportDetails {
        VkSurfaceCapabilitiesKHR capabilities;
        std::vector<VkSurfaceFormatKHR> formats;
        std::vector<VkPresentModeKHR> presentModes;
    };
    SwapChainSupportDetails QuerySwapChainSupport(VkPhysicalDevice device);
private:
    VkSurfaceKHR m_Surface = nullptr;
    VkSwapchainKHR m_SwapChain = nullptr;

    VkExtent2D m_SwapChainExtent;
    uint32_t m_Width = 0u, m_Height = 0u;
    bool m_Resized = false;

    VkFormat m_SwapChainImageFormat;

    std::vector<VkImage> m_SwapChainImages;
    std::vector<VkImageView> m_SwapChainImageViews;
    std::vector<VkFramebuffer> m_SwapChainFramebuffers;
};

}