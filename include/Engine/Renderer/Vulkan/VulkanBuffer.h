#pragma once

#include "volk.h"
#include "Engine/Renderer/Vulkan/VulkanDevice.h"

class VulkanBuffer {
private:
    VkBuffer buffer = VK_NULL_HANDLE;
    VmaAllocation allocation = VK_NULL_HANDLE;
    VkDeviceMemory memory;

    VulkanDevice* device;

    VkDeviceSize size{0};
    uint8_t* mappedData{nullptr};
    bool mapped = false;

    void map();
    void unmap();
    void flush();
public:
    VulkanBuffer(VulkanDevice* device, VkDeviceSize size, VkBufferUsageFlags buffer_usage, VmaMemoryUsage memory_usage, VmaAllocationCreateFlags flags = VMA_ALLOCATION_CREATE_MAPPED_BIT);
    ~VulkanBuffer();

    VkBuffer getBuffer();

    void update(const void* data, size_t size, size_t offset);

    void transferDataFrom(VulkanBuffer* otherBuffer);
};