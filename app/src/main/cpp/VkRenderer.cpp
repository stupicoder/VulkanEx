//
// Created by huhcmLocal on 2024-07-15.
//

#include <cassert>
#include <array>
#include <vector>
#include <iomanip>

#include "VkRenderer.h"
#include "VkUtil.h"
#include "AndroidOut.h"

using namespace std;

VkRenderer::VkRenderer(ANativeWindow* window) {
    // ================================================================================
    // 1. VkInstance 생성
    // ================================================================================
    VkApplicationInfo applicationInfo{
        .sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
        .pApplicationName = "Vulkan Ex",
        .applicationVersion = VK_MAKE_API_VERSION(0, 0, 1, 0),
        .apiVersion = VK_MAKE_API_VERSION(0, 1, 3, 0)
    };

    uint32_t  instanceLayerCount;
    VK_CHECK_ERROR(vkEnumerateInstanceLayerProperties(&instanceLayerCount, nullptr));

    vector<VkLayerProperties> instanceLayerProperties(instanceLayerCount);
    VK_CHECK_ERROR(vkEnumerateInstanceLayerProperties(&instanceLayerCount, instanceLayerProperties.data()));

    vector<const char*> instanceLayerNames;
    for (const auto& layerProperty : instanceLayerProperties) {
        instanceLayerNames.push_back(layerProperty.layerName);
    }

    uint32_t instanceExtensionCount = 0;
    VK_CHECK_ERROR(vkEnumerateInstanceExtensionProperties(nullptr, &instanceExtensionCount, nullptr));

    vector<VkExtensionProperties> instanceExtensionProperties(instanceExtensionCount);
    VK_CHECK_ERROR(vkEnumerateInstanceExtensionProperties(nullptr, &instanceExtensionCount, instanceExtensionProperties.data()));

    vector<const char*> instanceExtensionNames;
    for(const auto& properties : instanceExtensionProperties)
    {
        if (properties.extensionName == string("VK_KHR_surface") ||
            properties.extensionName == string("VK_KHR_android_surface"))
        {
            instanceExtensionNames.push_back(properties.extensionName);
        }
    }
    assert(instanceExtensionNames.size() == 2);

    VkInstanceCreateInfo instanceCreateInfo{
        .sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
        .pApplicationInfo = &applicationInfo,
        .enabledLayerCount = instanceLayerCount,
        .ppEnabledLayerNames = instanceLayerNames.data(),
        .enabledExtensionCount = static_cast<uint32_t>(instanceExtensionNames.size()),
        .ppEnabledExtensionNames = instanceExtensionNames.data()
    };

    VK_CHECK_ERROR(vkCreateInstance(&instanceCreateInfo, nullptr, &mInstance));

    // ================================================================================
    // 2. VkPhysicalDevice 선택
    // ================================================================================
    uint32_t physicalDeviceCount;
    VK_CHECK_ERROR(vkEnumeratePhysicalDevices(mInstance, &physicalDeviceCount, nullptr));

    vector<VkPhysicalDevice> physicalDevices(physicalDeviceCount);
    VK_CHECK_ERROR(vkEnumeratePhysicalDevices(mInstance, &physicalDeviceCount, physicalDevices.data()));

    // 간단한 예제를 위해 첫 번째 VkPhysicalDevice를 사용합니다.
    mPhysicalDevice = physicalDevices[0];

    VkPhysicalDeviceProperties physicalDeviceProperties;
    vkGetPhysicalDeviceProperties(mPhysicalDevice, &physicalDeviceProperties);

    aout << "Selected Physical Device Information " << endl;
    aout << setw(16) << left << " - Device Name: " << string_view(physicalDeviceProperties.deviceName) << endl;
    aout << setw(16) << left << " - Device Type: " << vkToString(physicalDeviceProperties.deviceType) << endl;
    aout << std::hex;
    aout << setw(16) << left << " - Device ID: " << physicalDeviceProperties.deviceID << endl;
    aout << setw(16) << left << " - Vendor ID: " << physicalDeviceProperties.vendorID << endl;
    aout << std::dec;
    aout << setw(16) << left << " - API Version: "
         << VK_API_VERSION_MAJOR(physicalDeviceProperties.apiVersion) << "."
         << VK_API_VERSION_MINOR(physicalDeviceProperties.apiVersion);
    aout << setw(16) << left << " - Driver Version: "
         << VK_API_VERSION_MAJOR(physicalDeviceProperties.driverVersion) << "."
         << VK_API_VERSION_MINOR(physicalDeviceProperties.driverVersion);

    // ================================================================================
    // 3. VkDevice 생성
    // ================================================================================
    uint32_t queueFamilyPropertiesCount;
    vkGetPhysicalDeviceQueueFamilyProperties(mPhysicalDevice, &queueFamilyPropertiesCount, nullptr);

    vector<VkQueueFamilyProperties> queueFamilyProperties(queueFamilyPropertiesCount);
    vkGetPhysicalDeviceQueueFamilyProperties(mPhysicalDevice, &queueFamilyPropertiesCount, queueFamilyProperties.data());

    for (mQueueFamilyIndex = 0; mQueueFamilyIndex < queueFamilyPropertiesCount; ++mQueueFamilyIndex)
    {
        if (queueFamilyProperties[mQueueFamilyIndex].queueFlags & VK_QUEUE_GRAPHICS_BIT)
        {
            break;
        }
    }

    const vector<float> queuePriorities{ 1.0 };
    VkDeviceQueueCreateInfo deviceQueueCreateInfo{
        .sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
        .queueFamilyIndex = mQueueFamilyIndex,
        .queueCount = 1,
        .pQueuePriorities = queuePriorities.data()
    };

    uint32_t deviceExtensionCount = 0;
    VK_CHECK_ERROR(vkEnumerateDeviceExtensionProperties(mPhysicalDevice, nullptr, &deviceExtensionCount, nullptr));

    vector<VkExtensionProperties> deviceExtentsionProperties(deviceExtensionCount);
    VK_CHECK_ERROR(vkEnumerateDeviceExtensionProperties(mPhysicalDevice, nullptr, &deviceExtensionCount, deviceExtentsionProperties.data()));

    vector<const char*> deviceExtensionNames;
    for(const auto& properties : deviceExtentsionProperties)
    {
        if (properties.extensionName == string("VK_KHR_swapchain"))
        {
            deviceExtensionNames.push_back(properties.extensionName);
        }
    }
    assert(deviceExtensionNames.size() == 1);

    VkDeviceCreateInfo deviceCreateInfo{
        .sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
        .queueCreateInfoCount = 1,
        .pQueueCreateInfos = &deviceQueueCreateInfo,
        .enabledExtensionCount = static_cast<uint32_t>(deviceExtensionNames.size()),
        .ppEnabledExtensionNames = deviceExtensionNames.data()
    };

    VK_CHECK_ERROR(vkCreateDevice(mPhysicalDevice, &deviceCreateInfo, nullptr, &mDevice));
    vkGetDeviceQueue(mDevice, mQueueFamilyIndex, 0, &mQueue);

    // ================================================================================
    // 4. VkSurface 생성
    // ================================================================================
    VkAndroidSurfaceCreateInfoKHR surfaceCreateInfo{
        .sType = VK_STRUCTURE_TYPE_ANDROID_SURFACE_CREATE_INFO_KHR,
        .window = window
    };

    VK_CHECK_ERROR(vkCreateAndroidSurfaceKHR(mInstance, &surfaceCreateInfo, nullptr, &mSurface));

    VkBool32 supported;
    VK_CHECK_ERROR(vkGetPhysicalDeviceSurfaceSupportKHR(mPhysicalDevice, mQueueFamilyIndex, mSurface, &supported));

    assert(supported);
}

VkRenderer::~VkRenderer() {
    vkDestroySurfaceKHR(mInstance, mSurface, nullptr);
    vkDestroyDevice(mDevice, nullptr);
    vkDestroyInstance(mInstance, nullptr);
}
