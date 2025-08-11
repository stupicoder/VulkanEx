#pragma once

#include <Vulkan/vulkan.h>
#include <vector>

class VkRenderer
{
public:
    explicit VkRenderer(void* InWindowHandle);
    ~VkRenderer();

    void Render();

private:
    VkInstance mInstance;
#if _DEBUG
    VkDebugUtilsMessengerEXT mDebugMessenger;
#endif
    VkPhysicalDevice mPhysicalDevice;
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
    VkClearColorValue mClearColorValue{.float32{0.6431, 0.7765, 0.2235, 1.0}};
    VkSemaphore mSemaphore;
    std::vector<VkImageView> mSwapchainImageViews;
    VkRenderPass mRenderPass;

    std::vector<VkFramebuffer> mFramebuffers;
};