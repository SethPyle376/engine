#pragma once

#include "volk.h"
#include "Engine/Renderer/Vulkan/VulkanDevice.h"

class VulkanBuffer {
private:
    VkBuffer buffer;

    VulkanDevice* device;
public:
    VulkanBuffer(VulkanDevice* device);
};