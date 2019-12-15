#include "Engine/Renderer/Vulkan/VulkanUtils.h"

struct VulkanDevice {
    VkPhysicalDevice physicalDevice;
    VkDevice logicalDevice;

    VkPhysicalDeviceProperties properties;
    VkPhysicalDeviceFeatures features;
    VkPhysicalDeviceFeatures enabledFeatures;
    VkPhysicalDeviceMemoryProperties memoryProperties;

    struct {
        uint32_t graphics;
        uint32_t compute;
        uint32_t transfer;
    };

    VulkanDevice(VkPhysicalDevice physicalDevice) {
        this->physicalDevice = physicalDevice;

        vkGetPhysicalDeviceProperties(physicalDevice, &properties);
        vkGetPhysicalDeviceFeatures(physicalDevice, &features);
        vkGetPhysicalDeviceMemoryProperties(physicalDevice, &memoryProperties);

        
    }
};