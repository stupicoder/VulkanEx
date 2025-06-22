#pragma once

#include <Vulkan/vulkan.h>

class VkRenderer
{
public:
    VkRenderer(void* InWindowHandle);
    ~VkRenderer();

    void Render();

private:
    VkInstance mInstance;
    VkPhysicalDevice mPhysicalDevice;
    uint32_t mQueueFamilyIndex;
    VkDevice mDevice;
    VkQueue mQueue;
    VkSurfaceKHR mSurface;
};