//
// Created by Huh on 2025-06-09.
//

#ifndef PRACTICE_VKRENDERER_H
#define PRACTICE_VKRENDERER_H

#include <vulkan/vulkan.h>
#include <vector>

class VkRenderer {
public:
    explicit VkRenderer(ANativeWindow* window);
    ~VkRenderer();

    void render();

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
    VkSemaphore mSemaphore;
    std::vector<VkImageView> mSwapchainImageViews;
    VkRenderPass mRenderPass;
};


#endif //PRACTICE_VKRENDERER_H
