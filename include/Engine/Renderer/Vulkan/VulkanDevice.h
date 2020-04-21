#pragma once

#include <vector>

#include "Engine/common/VulkanCommon.h"
#include "vk_mem_alloc.h"

#include "spdlog/spdlog.h"

class VulkanDevice {
    VkInstance instance;
    VkPhysicalDevice physicalDevice;
    VkDevice logicalDevice;

    VkPhysicalDeviceProperties properties;
    VkPhysicalDeviceFeatures features;
    VkPhysicalDeviceFeatures enabledFeatures;
    VkPhysicalDeviceMemoryProperties memoryProperties;

    VkQueue graphicsQueue;

    std::vector<std::string> supportedExtensions;
    std::vector<VkQueueFamilyProperties> queueFamilyProperties;

    VkCommandPool commandPool = VK_NULL_HANDLE;

    VmaAllocator allocator;

    bool enableDebugMarkers = false;

    uint32_t getQueueFamilyIndex(VkQueueFlagBits queueFlags);

    bool extensionSupported(std::string extension);

    VkCommandPool createCommandPool(uint32_t queueFamilyIndex, VkCommandPoolCreateFlags createFlags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT);

public:

    struct {
        uint32_t graphics;
        uint32_t compute;
        uint32_t transfer;
    } queueFamilyIndices;

    VulkanDevice(VkInstance instance, VkPhysicalDevice physicalDevice);

    ~VulkanDevice();

    // TODO: Consider using explicit transfer queue
    VkResult createLogicalDevice(VkPhysicalDeviceFeatures enabledFeatures, std::vector<const char*> enabledExtensions,
                                    void* pNextChain, bool useSwapChain = true, VkQueueFlags requestedQueueTypes = VK_QUEUE_GRAPHICS_BIT | VK_QUEUE_COMPUTE_BIT);
    void initAllocator();
    VkCommandPool getCommandPool();
    VkPhysicalDevice getPhysicalDevice();
    VkDevice getDevice();
    VkQueue getGraphicsQueue();
};

