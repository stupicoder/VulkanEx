//
// Created by huhcmLocal on 2024-07-15.
//

#ifndef VULKANEX_VKRENDERER_H
#define VULKANEX_VKRENDERER_H

#include <vector>

#define VK_USE_PLATFORM_ANDROID_KHR
#include <vulkan/vulkan.h>

class VkRenderer {
public:
    explicit VkRenderer(ANativeWindow* window);
    ~VkRenderer();

private:
    VkInstance mInstance;
    VkPhysicalDevice mPhysicalDevice;
    uint32_t mQueueFamilyIndex;
    VkDevice mDevice;
    VkQueue mQueue;
    VkSurfaceKHR mSurface;
    VkSwapchainKHR mSwapchain;
    std::vector<VkImage> mSwapchainImages;
};


#endif //VULKANEX_VKRENDERER_H
