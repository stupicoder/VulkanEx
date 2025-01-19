//
// Created by huhcmLocal on 2024-07-15.
//

#ifndef VULKANEX_VKRENDERER_H
#define VULKANEX_VKRENDERER_H

#include <vector>

//#define VK_USE_PLATFORM_ANDROID_KHR
#include <vulkan/vulkan.h>

class VkRenderer {
public:
    explicit VkRenderer(ANativeWindow* window);
    ~VkRenderer();

    void render();

private:
    VkInstance mInstance;
    VkPhysicalDevice mPhysicalDevice;
    VkPhysicalDeviceMemoryProperties mPhysicalDeviceMemoryProperties;
    uint32_t mQueueFamilyIndex;
    VkDevice mDevice;
    VkQueue mQueue;
    VkSurfaceKHR mSurface;
    VkSwapchainKHR mSwapchain;
    std::vector<VkImage> mSwapchainImages;
    VkExtent2D mSwapchainImageExtent;
    VkCommandPool mCommandPool;
    VkCommandBuffer mCommandBuffer;
    VkFence mFence;
    VkClearValue mClearValue{.color{.float32{0.15, 0.15, 0.15, 1.0}}};
    VkSemaphore mSemaphore;
    std::vector<VkImageView> mSwapchainImageViews;
    VkRenderPass mRenderPass;
    std::vector<VkFramebuffer> mFramebuffers;
    VkShaderModule mVertexShaderModule;
    VkShaderModule mFragmentShaderModule;
    VkDescriptorSetLayout mDescriptorSetLayout;
    VkPipelineLayout mPipelineLayout;
    VkPipeline mPipeline;
    VkBuffer mVertexBuffer;
    VkDeviceMemory mVertexMemory;
};


#endif //VULKANEX_VKRENDERER_H
