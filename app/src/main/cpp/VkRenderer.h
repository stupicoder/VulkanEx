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

    void Render();

private:
    VkInstance mInstance;
    VkPhysicalDevice mPhysicalDevice;
    uint32_t mQueueFamilyIndex;
    VkDevice mDevice;
    VkQueue mQueue;
    VkSurfaceKHR mSurface;
    VkSwapchainKHR mSwapchain;
    std::vector<VkImage> mSwapchainImages;
    VkCommandPool mCommandPool;
    VkCommandBuffer mCommandBuffer;
    VkFence mFence;
    VkClearColorValue mClearColorValue{.float32{0.6431, 0.7765, 0.2235, 1.0}};
};


#endif //VULKANEX_VKRENDERER_H
