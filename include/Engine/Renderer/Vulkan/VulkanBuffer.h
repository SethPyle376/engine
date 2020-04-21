#pragma once

#include "volk.h"
#include "Engine/Renderer/Vulkan/VulkanDevice.h"

class VulkanBuffer {
private:
    VkBuffer buffer;
    VmaAllocation allocation;
    VkDeviceMemory memory;

    VulkanDevice* device;
public:
    VulkanBuffer(VulkanDevice* device, VkDeviceSize size, VkBufferUsageFlags buffer_usage, VmaMemoryUsage memory_usage, VmaAllocationCreateFlags flags = VMA_ALLOCATION_CREATE_MAPPED_BIT);
    ~VulkanBuffer();
};