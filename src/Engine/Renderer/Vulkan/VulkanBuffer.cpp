#include "Engine/Renderer/Vulkan/VulkanBuffer.h"

VulkanBuffer::VulkanBuffer(VulkanDevice* device, VkDeviceSize size, VkBufferUsageFlags buffer_usage, VmaMemoryUsage memory_usage, VmaAllocationCreateFlags flags) {
    this->device = device;
    VkBufferCreateInfo bufferInfo = {};
    bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferInfo.usage = buffer_usage;
    bufferInfo.size = size;

    VmaAllocationCreateInfo memoryInfo{};
    memoryInfo.flags = flags;
    memoryInfo.usage = memory_usage;

    VmaAllocationInfo allocationInfo{};
    
    VkResult result = vmaCreateBuffer(device->getAllocator(), &bufferInfo, &memoryInfo, &buffer, &allocation, &allocationInfo);

    if (result != VK_SUCCESS) {
        spdlog::error("failed to create vulkan buffer");
    } else {
        spdlog::debug("created vulkan buffer");
    }

    memory = allocationInfo.deviceMemory;
}

VulkanBuffer::~VulkanBuffer() {
    if (buffer != VK_NULL_HANDLE && allocation != VK_NULL_HANDLE) {
        vmaDestroyBuffer(device->getAllocator(), buffer, allocation);
    }
}