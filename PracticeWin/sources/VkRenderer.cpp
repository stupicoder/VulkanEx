#include "VkRenderer.h"
#include "VkUtils.h"

#include <iostream>
#include <vector>
#include <array>
#include <iomanip>

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

void SelectPhysicalDevice(VkInstance& InInstance, VkPhysicalDevice& OutDevice)
{
    uint32_t physicalDeviceCount;
    VK_CHECK_ERROR(vkEnumeratePhysicalDevices(InInstance, &physicalDeviceCount, nullptr));

    vector<VkPhysicalDevice> physicalDevices(physicalDeviceCount);
    VK_CHECK_ERROR(vkEnumeratePhysicalDevices(InInstance, &physicalDeviceCount, physicalDevices.data()));

    auto PrintPhysicalDeviceProperties = [](const VkPhysicalDeviceProperties& InProperties){
        cout << setw(16) << left << " - Device Name: "
            << string_view(InProperties.deviceName) << endl;
        cout << setw(16) << left << " - Device Type: "
            << vkToString(InProperties.deviceType) << endl;
        cout << std::hex;
        cout << setw(16) << left << " - Device ID: " << InProperties.deviceID << endl;
        cout << setw(16) << left << " - Vendor ID: " << InProperties.vendorID << endl;
        cout << std::dec;
        cout << setw(16) << left << " - API Version: "
            << VK_API_VERSION_MAJOR(InProperties.apiVersion) << "."
            << VK_API_VERSION_MINOR(InProperties.apiVersion);
        cout << setw(16) << left << " - Driver Version: "
            << VK_API_VERSION_MAJOR(InProperties.driverVersion) << "."
            << VK_API_VERSION_MINOR(InProperties.driverVersion);
    };

    OutDevice = physicalDevices[0];

    for(uint32_t i = 0; i < physicalDeviceCount; ++i)
    {
        cout << "physical devices [" << i << "]" << endl;
        VkPhysicalDeviceProperties physicalDeviceProperties;
        vkGetPhysicalDeviceProperties(physicalDevices[i], &physicalDeviceProperties);
        PrintPhysicalDeviceProperties(physicalDeviceProperties);
    }
    cout << "Select Physical Device 0" << endl;
}

VkRenderer::VkRenderer()
{
    CreateInstance(mInstance);
    SelectPhysicalDevice(mInstance, mPhysicalDevice);
}

VkRenderer::~VkRenderer()
{
    vkDestroyInstance(mInstance, nullptr);
}

void VkRenderer::Render()
{
    //std::cout << "Render" << std::endl; 
}
