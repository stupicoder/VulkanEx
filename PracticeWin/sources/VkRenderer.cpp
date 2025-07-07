#include "VkRenderer.h"
#include "VkUtils.h"

#include <iostream>
#include <vector>
#include <array>
#include <iomanip>
#include <cassert>
#include <windows.h>
#if _WIN32
#include <vulkan/vulkan_win32.h>
#endif

using namespace std;

#if _DEBUG

const char* vkSeverityToString(const VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity)
{
    if (messageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT)
    {
        return "[Vulkan_error] ";
    }
    else if (messageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT)
    {
        return "[Vulkan_warning] ";
    }
    else if (messageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT)
    {
        return "[Vulkan_info] ";
    }

    return "";
}

VKAPI_ATTR VkBool32 VKAPI_CALL DebugCallback(
    VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
    VkDebugUtilsMessageTypeFlagsEXT messageType,
    const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
    void* pUserData)
{
    cout << vkSeverityToString(messageSeverity) << pCallbackData->pMessage << endl;
    return VK_FALSE;
}

VkResult CreateDebugUtilsMessengerEXT(VkInstance instance,
    const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
    const VkAllocationCallbacks* pAllocator,
    VkDebugUtilsMessengerEXT* pMessenger) {
    
    auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(
        instance, "vkCreateDebugUtilsMessengerEXT");

    if (func != nullptr)
        return func(instance, pCreateInfo, pAllocator, pMessenger);
    else
        return VK_ERROR_EXTENSION_NOT_PRESENT;
}

void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT messenger,
    const VkAllocationCallbacks* pAllocator) {
    
    auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(
        instance, "vkDestroyDebugUtilsMessengerEXT");

    if (func != nullptr)
        func(instance, messenger, pAllocator);
}
#endif

void CreateInstance(VkInstance& OutInstance
#if _DEBUG
    , VkDebugUtilsMessengerEXT InDebugMessenger
#endif
    )
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
        if (property.layerName == string("VK_LAYER_EOS_Overlay") ||
            property.layerName == string("VK_LAYER_RENDERDOC_Capture")
#if !_DEBUG
            || property.layerName == string("VK_LAYER_KHRONOS_validation")
            || property.layerName == string("VK_LAYER_LUNARG_monitor")
#endif
            )
        {
            cout << "except- " << property.layerName <<  endl;
            continue;
        }
        instanceLayerNames.push_back(property.layerName);
        cout << "\t" << property.layerName << endl;
    }

    uint32_t instanceExtensionCount;
    VK_CHECK_ERROR(vkEnumerateInstanceExtensionProperties(nullptr, &instanceExtensionCount, nullptr));

    vector<VkExtensionProperties> instanceExtentionProperties(instanceExtensionCount);
    VK_CHECK_ERROR(vkEnumerateInstanceExtensionProperties(nullptr, &instanceExtensionCount, instanceExtentionProperties.data()));

    cout << "instance extension names" << endl;
    vector<const char*> instanceExtensionNames;
    for (const VkExtensionProperties& properties : instanceExtentionProperties)
    {
        if (properties.extensionName == string("VK_KHR_surface") ||
            properties.extensionName == string("VK_KHR_win32_surface")
#if _DEBUG
            || properties.extensionName == string(VK_EXT_DEBUG_UTILS_EXTENSION_NAME)  // "VK_EXT_debug_utils"
#endif
            )
        {
            instanceExtensionNames.push_back(properties.extensionName);
            cout << "include-" << properties.extensionName << endl;
            continue;
        }

        cout << "\t" << properties.extensionName << endl;
    }
    assert(instanceExtensionNames.size() >= 2);

#if _DEBUG
    VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo{};
    debugCreateInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
    debugCreateInfo.messageSeverity =
        //VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
        VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
        VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
    debugCreateInfo.messageType =
        VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
        VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
        VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
    debugCreateInfo.pfnUserCallback = DebugCallback;
    debugCreateInfo.pUserData = nullptr;
#endif

    VkInstanceCreateInfo instanceCreateInfo{
        .sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
#if _DEBUG
        .pNext = (VkDebugUtilsMessengerCreateInfoEXT*)&debugCreateInfo,
#endif
        .pApplicationInfo = &applicationInfo,
        .enabledLayerCount = static_cast<uint32_t>(instanceLayerNames.size()),
        .ppEnabledLayerNames = instanceLayerNames.data(),
        .enabledExtensionCount = static_cast<uint32_t>(instanceExtensionNames.size()),
        .ppEnabledExtensionNames = instanceExtensionNames.data()
    };

    VK_CHECK_ERROR(vkCreateInstance(&instanceCreateInfo, nullptr, &OutInstance));

#if _DEBUG
    CreateDebugUtilsMessengerEXT(OutInstance, &debugCreateInfo, nullptr, &InDebugMessenger);
#endif
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

    uint32_t deviceExtensionCount;
    VK_CHECK_ERROR(vkEnumerateDeviceExtensionProperties(InPhysicalDevice, nullptr, &deviceExtensionCount, nullptr));

    vector<VkExtensionProperties> deviceExtensionProperties(deviceExtensionCount);
    VK_CHECK_ERROR(vkEnumerateDeviceExtensionProperties(InPhysicalDevice, nullptr, &deviceExtensionCount, deviceExtensionProperties.data()));

    cout << "device extension names" << endl;
    vector<const char*> deviceExtensionNames;
    for (const VkExtensionProperties& properties : deviceExtensionProperties)
    {
        if (properties.extensionName == string("VK_KHR_swapchain"))
        {
            deviceExtensionNames.push_back(properties.extensionName);
            cout << "include-" << properties.extensionName << endl;
            continue;
        }
        cout << "\t" << properties.extensionName << endl;
    }
    assert(deviceExtensionNames.size() >= 1);

    VkDeviceCreateInfo deviceCreateInfo{
        .sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
        .queueCreateInfoCount = 1,
        .pQueueCreateInfos = &deviceQueueCreateInfo,
        .enabledExtensionCount = static_cast<uint32_t>(deviceExtensionNames.size()),
        .ppEnabledExtensionNames = deviceExtensionNames.data()
    };

    VK_CHECK_ERROR(vkCreateDevice(InPhysicalDevice, &deviceCreateInfo, nullptr, &OutDevice));
    vkGetDeviceQueue(OutDevice, OutQueueFamilyIndex, 0, &OutQueue);
}

void CreateSurface(VkPhysicalDevice& InPhysicalDevice, uint32_t InQueueFamilyIndex, VkInstance& InInstance, void* InWindowHandle, VkSurfaceKHR& OutSurface)
{
#if _WIN32
    VkWin32SurfaceCreateInfoKHR surfaceCreateInfo{
        .sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR,
        .hwnd = (HWND)InWindowHandle
    };

    VK_CHECK_ERROR(vkCreateWin32SurfaceKHR(InInstance, &surfaceCreateInfo, nullptr, &OutSurface));

    VkBool32 supported;
    VK_CHECK_ERROR(vkGetPhysicalDeviceSurfaceSupportKHR(InPhysicalDevice, InQueueFamilyIndex, OutSurface, &supported));

    assert(supported);
#endif
}

#if _WIN32
void CreateSwapchain(VkPhysicalDevice& InPhysicalDevice, VkDevice& InDevice, VkSurfaceKHR& InSurface, VkSwapchainKHR& OutSwapchain, vector<VkImage>& OutSwapchainImages)
{
    VkSurfaceCapabilitiesKHR surfaceCapabilities;
    VK_CHECK_ERROR(vkGetPhysicalDeviceSurfaceCapabilitiesKHR(InPhysicalDevice, InSurface, &surfaceCapabilities));

    VkCompositeAlphaFlagBitsKHR compositeAlpha = VK_COMPOSITE_ALPHA_FLAG_BITS_MAX_ENUM_KHR;
    for (uint32_t i = 0; i <= 4; ++i)
    {
        if (uint32_t flag = 0x1u << i; surfaceCapabilities.supportedCompositeAlpha & flag)
        {
            compositeAlpha = static_cast<VkCompositeAlphaFlagBitsKHR>(flag);
            break;
        }
    }
    assert(compositeAlpha != VK_COMPOSITE_ALPHA_FLAG_BITS_MAX_ENUM_KHR);

    VkImageUsageFlags imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
    assert(surfaceCapabilities.supportedUsageFlags & imageUsage);

    uint32_t surfaceFormatCount = 0;
    VK_CHECK_ERROR(vkGetPhysicalDeviceSurfaceFormatsKHR(InPhysicalDevice, InSurface, &surfaceFormatCount, nullptr));

    vector<VkSurfaceFormatKHR> surfaceFormats(surfaceFormatCount);
    VK_CHECK_ERROR(vkGetPhysicalDeviceSurfaceFormatsKHR(InPhysicalDevice, InSurface, &surfaceFormatCount, surfaceFormats.data()));

    uint32_t surfaceFormatIndex = VK_FORMAT_MAX_ENUM;
    for(uint32_t i = 0; i != surfaceFormatCount; ++i)
    {
        if (surfaceFormats[i].format == VK_FORMAT_R8G8B8A8_UNORM)
        {
            surfaceFormatIndex = i;
            break;
        }
    }
    assert(surfaceFormatIndex != VK_FORMAT_MAX_ENUM);

    uint32_t presentModeCount;
    VK_CHECK_ERROR(vkGetPhysicalDeviceSurfacePresentModesKHR(InPhysicalDevice, InSurface, &presentModeCount, nullptr));

    vector<VkPresentModeKHR> presentModes(presentModeCount);
    VK_CHECK_ERROR(vkGetPhysicalDeviceSurfacePresentModesKHR(InPhysicalDevice, InSurface, &presentModeCount, presentModes.data()));

    uint32_t presentModeIndex = VK_PRESENT_MODE_MAX_ENUM_KHR;
    for (uint32_t i = 0; i != presentModeCount; ++i)
    {
        if (presentModes[i] == VK_PRESENT_MODE_FIFO_KHR)
        {
            presentModeIndex = i;
            break;
        }
    }
    assert(presentModeIndex != VK_PRESENT_MODE_MAX_ENUM_KHR);

    VkSwapchainCreateInfoKHR swapchainCreateInfo{
        .sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
        .surface = InSurface,
        .minImageCount = surfaceCapabilities.minImageCount,
        .imageFormat = surfaceFormats[surfaceFormatIndex].format,
        .imageColorSpace = surfaceFormats[surfaceFormatIndex].colorSpace,
        .imageExtent = surfaceCapabilities.currentExtent,
        .imageArrayLayers = 1,
        .imageUsage = imageUsage,
        .imageSharingMode = VK_SHARING_MODE_EXCLUSIVE,
        .preTransform = surfaceCapabilities.currentTransform,
        .compositeAlpha = compositeAlpha,
        .presentMode = presentModes[presentModeIndex]
    };

    VK_CHECK_ERROR(vkCreateSwapchainKHR(InDevice, &swapchainCreateInfo, nullptr, &OutSwapchain));

    uint32_t swapchainImageCount;
    VK_CHECK_ERROR(vkGetSwapchainImagesKHR(InDevice, OutSwapchain, &swapchainImageCount, nullptr));

    OutSwapchainImages.resize(swapchainImageCount);
    VK_CHECK_ERROR(vkGetSwapchainImagesKHR(InDevice, OutSwapchain, &swapchainImageCount, OutSwapchainImages.data()));
}
#endif

void CreateCommandPool(const uint32_t InQueueFamilyIndex, VkDevice& InDevice, VkCommandPool& OutCommandPool)
{
    VkCommandPoolCreateInfo commandPoolCreateInfo{
        .sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
        .flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT,
        .queueFamilyIndex = InQueueFamilyIndex
    };

    VK_CHECK_ERROR(vkCreateCommandPool(InDevice, &commandPoolCreateInfo, nullptr, &OutCommandPool));
}

void AllocCommandBuffer(VkDevice& InDevice, VkCommandPool& InCommandPool, VkCommandBuffer& OutCommandBuffer)
{
    VkCommandBufferAllocateInfo commandBufferAllocateInfo{
        .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
        .commandPool = InCommandPool,
        .level = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
        .commandBufferCount = 1
    };

    VK_CHECK_ERROR(vkAllocateCommandBuffers(InDevice, &commandBufferAllocateInfo, &OutCommandBuffer));
}

void RegistCommandBuffer(VkCommandBuffer& InCommandBuffer, vector<VkImage>& InSwapchainImages, VkQueue& InQueue)
{
    VkCommandBufferBeginInfo commandBufferBeginInfo{
        .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
        .flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT
    };

    VK_CHECK_ERROR(vkBeginCommandBuffer(InCommandBuffer, &commandBufferBeginInfo));

    for (VkImage& swapchainImage : InSwapchainImages)
    {
        VkClearColorValue clearColorValue{
            .float32 = {0.6431f, 0.7765f, 0.2235f, 1.0f}
        };

        VkImageSubresourceRange imageSubresourceRange{
            .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
            .baseMipLevel = 0,
            .levelCount = 1,
            .baseArrayLayer = 0,
            .layerCount = 1
        };

        vkCmdClearColorImage(InCommandBuffer, swapchainImage, VK_IMAGE_LAYOUT_UNDEFINED, &clearColorValue, 1, &imageSubresourceRange);

    }
    
    VK_CHECK_ERROR(vkEndCommandBuffer(InCommandBuffer));

    VkSubmitInfo submitInfo{
        .sType = VK_STRUCTURE_TYPE_SUBMIT_INFO,
        .commandBufferCount = 1,
        .pCommandBuffers = &InCommandBuffer
    };

    VK_CHECK_ERROR(vkQueueSubmit(InQueue, 1, &submitInfo, VK_NULL_HANDLE));
    VK_CHECK_ERROR(vkQueueWaitIdle(InQueue));
}

VkRenderer::VkRenderer(void* InWindowHandle)
{
    CreateInstance(mInstance
#if _DEBUG
        , mDebugMessenger
#endif
        );
    SelectPhysicalDevice(mInstance, mPhysicalDevice);
    CreateDevice(mPhysicalDevice, mQueueFamilyIndex, mQueue, mDevice);
    CreateSurface(mPhysicalDevice, mQueueFamilyIndex, mInstance, InWindowHandle, mSurface);
#if _WIN32
    CreateSwapchain(mPhysicalDevice, mDevice, mSurface, mSwapchain, mSwapchainImages);
#endif
    CreateCommandPool(mQueueFamilyIndex, mDevice, mCommandPool);
    AllocCommandBuffer(mDevice, mCommandPool, mCommandBuffer);
    RegistCommandBuffer(mCommandBuffer, mSwapchainImages, mQueue);
}

VkRenderer::~VkRenderer()
{
    vkFreeCommandBuffers(mDevice, mCommandPool, 1, &mCommandBuffer);
    vkDestroyCommandPool(mDevice, mCommandPool, nullptr);
    vkDestroySwapchainKHR(mDevice, mSwapchain, nullptr);
    vkDestroySurfaceKHR(mInstance, mSurface, nullptr);
    vkDestroyDevice(mDevice, nullptr);
#if _DEBUG
    DestroyDebugUtilsMessengerEXT(mInstance, mDebugMessenger, nullptr);
#endif
    vkDestroyInstance(mInstance, nullptr);
}

void VkRenderer::Render()
{
    uint32_t swapchainImageIndex;
    VK_CHECK_ERROR(vkAcquireNextImageKHR(mDevice, mSwapchain, UINT64_MAX, VK_NULL_HANDLE, VK_NULL_HANDLE, &swapchainImageIndex));

    VkPresentInfoKHR presentInfo{
        .sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR,
        .swapchainCount = 1,
        .pSwapchains = &mSwapchain,
        .pImageIndices = &swapchainImageIndex
    };

    VK_CHECK_ERROR(vkQueuePresentKHR(mQueue, &presentInfo));
    VK_CHECK_ERROR(vkQueueWaitIdle(mQueue));
}
