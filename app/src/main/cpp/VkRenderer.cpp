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
    uint32_t  layerCount;
    VK_CHECK_ERROR(vkEnumerateInstanceLayerProperties(&layerCount, nullptr));

    vector<VkLayerProperties> layerProps(layerCount);
    VK_CHECK_ERROR(vkEnumerateInstanceLayerProperties(&layerCount, layerProps.data()));

    for (const auto& layerProp : layerProps) {
        aout << layerProp.layerName << endl;
    }
}

VkRenderer::~VkRenderer() {
}
