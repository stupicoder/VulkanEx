//
// Created by huhcmLocal on 2024-07-15.
//

#ifndef VULKANEX_VKRENDERER_H
#define VULKANEX_VKRENDERER_H

#include <vulkan/vulkan.h>

class VkRenderer {
public:
    VkRenderer();
    ~VkRenderer();

private:
    VkInstance mInstance;
    VkPhysicalDevice mPhysicalDevice;
    uint32_t mQueueFamilyIndex;
    VkDevice mDevice;
    VkQueue mQueue;
};


#endif //VULKANEX_VKRENDERER_H
