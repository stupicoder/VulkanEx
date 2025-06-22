#include "VkRenderer.h"
#include "VkUtils.h"

#include <iostream>
#include <vector>

using namespace std;

VkRenderer::VkRenderer()
{
    uint32_t instanceLayerCount;
    VK_CHECK_ERROR(vkEnumerateInstanceLayerProperties(&instanceLayerCount, nullptr));
    
    vector<VkLayerProperties> instanceLayerProperties(instanceLayerCount);
    VK_CHECK_ERROR(vkEnumerateInstanceLayerProperties(&instanceLayerCount, instanceLayerProperties.data()));

    for (const VkLayerProperties& layerProp : instanceLayerProperties)
    {
        cout << layerProp.layerName << endl;
    }
}

VkRenderer::~VkRenderer()
{

}

void VkRenderer::Render()
{
    //std::cout << "Render" << std::endl; 
}
