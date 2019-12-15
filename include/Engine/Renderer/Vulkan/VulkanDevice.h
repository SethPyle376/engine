#pragma once

#include "volk.h"

struct VulkanDevice {
    VkPhysicalDevice physicalDevice;
    VkDevice logicalDevice;

    VkPhysicalDeviceProperties properties;
    VkPhysicalDeviceFeatures features;
    VkPhysicalDeviceFeatures enabledFeatures;
    VkPhysicalDeviceMemoryProperties memoryProperties;

    std::vector<std::string> supportedExtensions;
    std::vector<VkQueueFamilyProperties> queueFamilyProperties;

    VkCommandPool commandPool = VK_NULL_HANDLE;

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

        uint32_t queueFamilyCount;
        vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, nullptr);
        queueFamilyProperties.resize(queueFamilyCount);
        vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, queueFamilyProperties.data());

        uint32_t extensionCount = 0;
        vkEnumerateDeviceExtensionProperties(physicalDevice, nullptr, &extensionCount, nullptr);

        if (extensionCount > 0) {
            std::vector<VkExtensionProperties> extensions(extensionCount);
            if (vkEnumerateDeviceExtensionProperties(physicalDevice, nullptr, &extensionCount, &extensions.front()) == VK_SUCCESS) {
                for (auto extension : extensions) {
                    supportedExtensions.push_back(extension.extensionName);
                }
            }
        }
    }

    ~VulkanDevice() {
        if (commandPool) {
            vkDestroyCommandPool(logicalDevice, commandPool, nullptr);
        }

        if (logicalDevice) {
            vkDestroyDevice(logicalDevice, nullptr);
        }
    }

    VkResult createLogicalDevice() {
        std::vector<VkDeviceQueueCreateInfo> queueCreateInfos{};

        return VK_SUCCESS;
    }
};