//
// Created by Huh on 2025-06-09.
//

#ifndef PRACTICE_VKRENDERER_H
#define PRACTICE_VKRENDERER_H

#include <vulkan/vulkan.h>

class VkRenderer {
public:
    VkRenderer(ANativeWindow* window);
    ~VkRenderer();

private:
    VkInstance mInstance;
    VkPhysicalDevice mPhysicalDevice;
    uint32_t mQueueFamilyIndex;
    VkDevice mDevice;
    VkQueue mQueue;
    VkSurfaceKHR mSurface;
};


#endif //PRACTICE_VKRENDERER_H
