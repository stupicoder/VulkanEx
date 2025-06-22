#include "VkRenderer.h"
#include "VkUtils.h"

#include <iostream>
#include <vector>

using namespace std;

void CreateInstance(VkInstance& OutInstance)
{
    VkApplicationInfo applicationInfo{
        .sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
        .pApplicationName = "Practice Vulkan",
        .applicationVersion = VK_MAKE_API_VERSION(0, 0, 1, 0),
        .apiVersion = VK_MAKE_API_VERSION(0, 1, 3, 0)
    };

    uint32_t instanceLayerCount;
    VK_CHECK_ERROR(vkEnumerateInstanceLayerProperties(&instanceLayerCount, nullptr));
    
    vector<VkLayerProperties> instanceLayerProperties(instanceLayerCount);
    VK_CHECK_ERROR(vkEnumerateInstanceLayerProperties(&instanceLayerCount, instanceLayerProperties.data()));

    cout << "instance layer names" << endl;
    vector<const char*> instanceLayerNames;
    for (const VkLayerProperties& property : instanceLayerProperties)
    {
        if (property.layerName == string("VK_LAYER_EOS_Overlay"))
        {
            cout << "\t" << "except--" << property.layerName <<  endl;
            continue;
        }
        instanceLayerNames.push_back(property.layerName);
        cout << "\t" << property.layerName << endl;
    }


    VkInstanceCreateInfo instanceCreateInfo{
        .sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
        .pApplicationInfo = &applicationInfo,
        .enabledLayerCount = static_cast<uint32_t>(instanceLayerNames.size()),
        .ppEnabledLayerNames = instanceLayerNames.data()
    };

    VK_CHECK_ERROR(vkCreateInstance(&instanceCreateInfo, nullptr, &OutInstance));
}

VkRenderer::VkRenderer()
{
    CreateInstance(mInstance);
}

VkRenderer::~VkRenderer()
{
    vkDestroyInstance(mInstance, nullptr);
}

void VkRenderer::Render()
{
    //std::cout << "Render" << std::endl; 
}
