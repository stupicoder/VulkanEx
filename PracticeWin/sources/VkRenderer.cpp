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

void CreateDevice(VkPhysicalDevice& InPhysicalDevice, uint32_t& OutQueueFamilyIndex, VkQueue& OutQueue, VkDevice& OutDevice)
{
    uint32_t queueFamilyPropertiesCount;
    vkGetPhysicalDeviceQueueFamilyProperties(InPhysicalDevice, &queueFamilyPropertiesCount, nullptr);

    vector<VkQueueFamilyProperties> queueFamilyProperties(queueFamilyPropertiesCount);
    vkGetPhysicalDeviceQueueFamilyProperties(InPhysicalDevice, &queueFamilyPropertiesCount, queueFamilyProperties.data());

    auto PrintQueueFamilyFlags = [](VkQueueFlags& InFlags){
        cout << "\tFlags " << InFlags << " -";
        if (InFlags & VK_QUEUE_GRAPHICS_BIT) { cout << " | Graphics"; }
        if (InFlags & VK_QUEUE_COMPUTE_BIT) { cout << " | Compute"; }
        if (InFlags & VK_QUEUE_TRANSFER_BIT) { cout << " | Transfer"; }
        if (InFlags & VK_QUEUE_SPARSE_BINDING_BIT) { cout << " | Sparse Binding"; }
        if (InFlags & VK_QUEUE_PROTECTED_BIT) { cout << " | Protected"; }
        if (InFlags & VK_QUEUE_VIDEO_DECODE_BIT_KHR) { cout << " | Video Decode"; }
        if (InFlags & VK_QUEUE_VIDEO_ENCODE_BIT_KHR) { cout << " | Video Encode"; }
        if (InFlags & VK_QUEUE_OPTICAL_FLOW_BIT_NV) { cout << " | Optical Flow"; }
        cout << endl;
    };

    cout << "queue family flags" << endl;
    OutQueueFamilyIndex = UINT32_MAX;
    for (uint32_t i = 0; i < queueFamilyPropertiesCount; ++i)
    {
        PrintQueueFamilyFlags(queueFamilyProperties[i].queueFlags);
        if (OutQueueFamilyIndex == UINT32_MAX && queueFamilyProperties[i].queueFlags & VK_QUEUE_GRAPHICS_BIT)
        {
            OutQueueFamilyIndex = i;
        }
    }

    const vector<float> queuePriorites{ 1.0f };
    VkDeviceQueueCreateInfo deviceQueueCreateInfo{
        .sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
        .queueFamilyIndex = OutQueueFamilyIndex,
        .queueCount = 1,
        .pQueuePriorities = queuePriorites.data()
    };

    VkDeviceCreateInfo deviceCreateInfo{
        .sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
        .queueCreateInfoCount = 1,
        .pQueueCreateInfos = &deviceQueueCreateInfo
    };

    VK_CHECK_ERROR(vkCreateDevice(InPhysicalDevice, &deviceCreateInfo, nullptr, &OutDevice));
    vkGetDeviceQueue(OutDevice, OutQueueFamilyIndex, 0, &OutQueue);
}

VkRenderer::VkRenderer()
{
    CreateInstance(mInstance);
    SelectPhysicalDevice(mInstance, mPhysicalDevice);
    CreateDevice(mPhysicalDevice, mQueueFamilyIndex, mQueue, mDevice);
}

VkRenderer::~VkRenderer()
{
    vkDestroyDevice(mDevice, nullptr);
    vkDestroyInstance(mInstance, nullptr);
}

void VkRenderer::Render()
{
    //std::cout << "Render" << std::endl; 
}
