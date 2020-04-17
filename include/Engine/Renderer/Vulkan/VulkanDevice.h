#pragma once

#include "volk.h"

class VulkanDevice {
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

    bool enableDebugMarkers = false;

    uint32_t getQueueFamilyIndex(VkQueueFlagBits queueFlags) {
        if (queueFlags & VK_QUEUE_COMPUTE_BIT)
        {
            for (uint32_t i = 0; i < static_cast<uint32_t>(queueFamilyProperties.size()); i++)
            {
                if ((queueFamilyProperties[i].queueFlags & queueFlags) && ((queueFamilyProperties[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) == 0))
                {
                    return i;
                    break;
                }
            }
        }

        if (queueFlags & VK_QUEUE_TRANSFER_BIT)
        {
            for (uint32_t i = 0; i < static_cast<uint32_t>(queueFamilyProperties.size()); i++)
            {
                if ((queueFamilyProperties[i].queueFlags & queueFlags) && ((queueFamilyProperties[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) == 0) && ((queueFamilyProperties[i].queueFlags & VK_QUEUE_COMPUTE_BIT) == 0))
                {
                    return i;
                    break;
                }
            }
        }

        for (uint32_t i = 0; i < static_cast<uint32_t>(queueFamilyProperties.size()); i++)
        {
            if (queueFamilyProperties[i].queueFlags & queueFlags)
            {
                return i;
                break;
            }
        }
        return 0;
    }

    bool extensionSupported(std::string extension) {
        return (std::find(supportedExtensions.begin(), supportedExtensions.end(), extension) != supportedExtensions.end());
    }

    VkCommandPool createCommandPool(uint32_t queueFamilyIndex, VkCommandPoolCreateFlags createFlags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT) {
        VkCommandPoolCreateInfo cmdPoolInfo = {};
        cmdPoolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
        cmdPoolInfo.queueFamilyIndex = queueFamilyIndex;
        cmdPoolInfo.flags = createFlags;
        VkCommandPool cmdPool;
        VkResult result = vkCreateCommandPool(logicalDevice, &cmdPoolInfo, nullptr, &cmdPool);

        if (result == VK_SUCCESS) {
            spdlog::info("vulkan command pool initialized successfully");
        } else {
            spdlog::debug("failed to initalize vulkan command pool");
        }
        return cmdPool;
    }

public:

    struct {
        uint32_t graphics;
        uint32_t compute;
        uint32_t transfer;
    } queueFamilyIndices;

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

    // TODO: Consider using explicit transfer queue
    VkResult createLogicalDevice(VkPhysicalDeviceFeatures enabledFeatures, std::vector<const char*> enabledExtensions,
                                    void* pNextChain, bool useSwapChain = true, VkQueueFlags requestedQueueTypes = VK_QUEUE_GRAPHICS_BIT | VK_QUEUE_COMPUTE_BIT) {
        std::vector<VkDeviceQueueCreateInfo> queueCreateInfos{};

        const float defaultQueuePriority(0.0f);

        if (requestedQueueTypes & VK_QUEUE_GRAPHICS_BIT) {
            queueFamilyIndices.graphics = getQueueFamilyIndex(VK_QUEUE_GRAPHICS_BIT);
            VkDeviceQueueCreateInfo queueInfo{};
            queueInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
            queueInfo.queueFamilyIndex = queueFamilyIndices.graphics;
            queueInfo.queueCount = 1;
            queueInfo.pQueuePriorities = &defaultQueuePriority;
            queueCreateInfos.push_back(queueInfo);
        } else {
            queueFamilyIndices.graphics = VK_NULL_HANDLE;
        }

        if (requestedQueueTypes & VK_QUEUE_COMPUTE_BIT) {
            queueFamilyIndices.compute = getQueueFamilyIndex(VK_QUEUE_COMPUTE_BIT);
            if (queueFamilyIndices.compute != queueFamilyIndices.graphics) {
                VkDeviceQueueCreateInfo queueInfo{};
                queueInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
                queueInfo.queueFamilyIndex = queueFamilyIndices.compute;
                queueInfo.queueCount = 1;
                queueInfo.pQueuePriorities = &defaultQueuePriority;
                queueCreateInfos.push_back(queueInfo);
            }
        } else {
            queueFamilyIndices.compute = queueFamilyIndices.graphics;
        }

        if (requestedQueueTypes & VK_QUEUE_TRANSFER_BIT)
        {
            queueFamilyIndices.transfer = getQueueFamilyIndex(VK_QUEUE_TRANSFER_BIT);
            if ((queueFamilyIndices.transfer != queueFamilyIndices.graphics) && (queueFamilyIndices.transfer != queueFamilyIndices.compute))
            {
                VkDeviceQueueCreateInfo queueInfo{};
                queueInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
                queueInfo.queueFamilyIndex = queueFamilyIndices.transfer;
                queueInfo.queueCount = 1;
                queueInfo.pQueuePriorities = &defaultQueuePriority;
                queueCreateInfos.push_back(queueInfo);
            }
        }
        else
        {
            // Else we use the same queue
            queueFamilyIndices.transfer = queueFamilyIndices.graphics;
        }

        std::vector<const char*> deviceExtensions(enabledExtensions);

        if (useSwapChain) {
            deviceExtensions.push_back(VK_KHR_SWAPCHAIN_EXTENSION_NAME);
        }

        VkDeviceCreateInfo deviceCreateInfo = {};
        deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
        deviceCreateInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
        deviceCreateInfo.pQueueCreateInfos = queueCreateInfos.data();
        deviceCreateInfo.pEnabledFeatures = &enabledFeatures;

        if (pNextChain) {
            VkPhysicalDeviceFeatures2 physicalDeviceFeatures2{};
            physicalDeviceFeatures2.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2;
            physicalDeviceFeatures2.features = enabledFeatures;
            physicalDeviceFeatures2.pNext = pNextChain;
            deviceCreateInfo.pEnabledFeatures = nullptr;
            deviceCreateInfo.pNext = &physicalDeviceFeatures2;
        }

        if (extensionSupported(VK_EXT_DEBUG_MARKER_EXTENSION_NAME)) {
            deviceExtensions.push_back(VK_EXT_DEBUG_MARKER_EXTENSION_NAME);
            enableDebugMarkers = true;
        }

        if (deviceExtensions.size() > 0) {
            deviceCreateInfo.enabledExtensionCount = (uint32_t)deviceExtensions.size();
            deviceCreateInfo.ppEnabledExtensionNames = deviceExtensions.data();
        }

        VkResult result = vkCreateDevice(physicalDevice, &deviceCreateInfo, nullptr, &logicalDevice);

        if (result == VK_SUCCESS) {
            spdlog::info("vulkan device initalized successfully");
            commandPool = createCommandPool(queueFamilyIndices.graphics);
        } else {
            spdlog::error("vulkan device failed to initialize");
        }

        this->enabledFeatures = enabledFeatures;

        vkGetDeviceQueue(logicalDevice, queueFamilyIndices.graphics, 0, &graphicsQueue);

        return result;
    }

    VkCommandPool getCommandPool() {
        return commandPool;
    }

    VkPhysicalDevice getPhysicalDevice() {
        return physicalDevice;
    }

    VkDevice getDevice() {
        return logicalDevice;
    }

    VkQueue getGraphicsQueue() {
        return graphicsQueue;
    }
};

