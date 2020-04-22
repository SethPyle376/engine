#include "Engine/Renderer/Vulkan/VulkanBuffer.h"

VulkanBuffer::VulkanBuffer(VulkanDevice* device, VkDeviceSize size, VkBufferUsageFlags buffer_usage, VmaMemoryUsage memory_usage, VmaAllocationCreateFlags flags) {
    this->device = device;
    this->size = size;
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

void VulkanBuffer::map() {
    if (!mapped && !mappedData) {
        VkResult result = vmaMapMemory(device->getAllocator(), allocation, reinterpret_cast<void**>(&mappedData));

        if (result != VK_SUCCESS) {
            spdlog::error("failed to map vulkan memory");
        } else {
            spdlog::debug("mapped vulkan memory");
        }

        mapped = true;
    }
}

void VulkanBuffer::unmap() {
    if (mapped) {
        vmaUnmapMemory(device->getAllocator(), allocation);
        mappedData = nullptr;
        mapped = false;
    }
}

void VulkanBuffer::flush() {
    VkResult result = vmaFlushAllocation(device->getAllocator(), allocation, 0, size);

    if (result != VK_SUCCESS) {
        spdlog::error("failed to flush vulkan memory");
    } else {
        spdlog::debug("flushed vulkan buffer");
    }
}

void VulkanBuffer::update(const void* data, size_t size, size_t offset) {
    const uint8_t* convertedData = reinterpret_cast<const uint8_t*>(data);
    map();
    std::copy(convertedData, convertedData + size, mappedData + offset);
    flush();
    unmap();
}

VkBuffer VulkanBuffer::getBuffer() {
    return buffer;
}