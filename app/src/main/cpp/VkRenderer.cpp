//
// Created by huhcmLocal on 2024-07-15.
//

#include <cassert>
#include <vector>

#include "VkRenderer.h"
#include "VkUtil.h"
#include "AndroidOut.h"

using namespace std;

VkRenderer::VkRenderer() {

    VkApplicationInfo applicationInfo{
        .sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
        .pApplicationName = "Vulkan Ex",
        .applicationVersion = VK_MAKE_API_VERSION(0, 0, 1, 0),
        .apiVersion = VK_MAKE_API_VERSION(0, 1, 3, 0)
    };

    uint32_t  instanceLayerCount;
    VK_CHECK_ERROR(vkEnumerateInstanceLayerProperties(&instanceLayerCount, nullptr));

    vector<VkLayerProperties> instancLayerProperties(instanceLayerCount);
    VK_CHECK_ERROR(vkEnumerateInstanceLayerProperties(&instanceLayerCount, instancLayerProperties.data()));

    vector<const char*> instanceLayerNames;
    for (const auto& layerProperty : instancLayerProperties) {
        instanceLayerNames.push_back(layerProperty.layerName);
    }

    VkInstanceCreateInfo instanceCreateInfo{
        .sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
        .pApplicationInfo = &applicationInfo,
        .enabledLayerCount = instanceLayerCount,
        .ppEnabledLayerNames = instanceLayerNames.data()
    };

    VK_CHECK_ERROR(vkCreateInstance(&instanceCreateInfo, nullptr, &mInstance));
}

VkRenderer::~VkRenderer() {
    vkDestroyInstance(mInstance, nullptr);
}
