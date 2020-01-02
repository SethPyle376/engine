#pragma once

#include "SDL_vulkan.h"
#include "spdlog/spdlog.h"

class VulkanSwapchain {
private:
    VkInstance instance;
    VkPhysicalDevice physicalDevice;
    VkDevice device;
    VkSurfaceKHR surface;
    VkSwapchainKHR swapchain;

public:
    VulkanSwapchain(VkInstance instance) {
        this->instance = instance;
    }
    
    ~VulkanSwapchain() {
        vkDestroySurfaceKHR(instance, surface, nullptr);
    }

    void connect(VkPhysicalDevice physicalDevice, VkDevice device) {
        this->physicalDevice = physicalDevice;
        this->device = device;
    }

    void initSurface(SDL_Window *window) {
        if (!SDL_Vulkan_CreateSurface(window, instance, &surface)) {
            spdlog::error("failed to create vulkan surface");
        } else {
            spdlog::debug("created vulkan surface");
        }
    }

    void create(uint32_t width, uint32_t height, bool vsync = false) {
        VkSwapchainKHR oldSwapchain = swapchain;

        VkSurfaceCapabilitiesKHR surfaceCapabilities;
        vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevice, surface, &surfaceCapabilities);

        uint32_t presentModeCount;
        vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, &presentModeCount, nullptr);

        if (presentModeCount <= 0) {
            spdlog::error("no present modes available");
        }

        std::vector<VkPresentModeKHR> presentModes(presentModeCount);

        vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, &presentModeCount, presentModes.data());

        VkExtent2D swapchainExtent = {};

        if (surfaceCapabilities.currentExtent.width == (uint32_t)- 1) {
            spdlog::debug("vulkan swapchain extent set to requested extent");
            swapchainExtent.width = width;
            swapchainExtent.height = height;
        } else {
            swapchainExtent = surfaceCapabilities.currentExtent;
            spdlog::debug("vulkan swapchain extent set to current surface extent");
        }

        //Fallback if no non-vsync options and no vsync requested
        VkPresentModeKHR swapchainPresentMode = VK_PRESENT_MODE_FIFO_KHR;

        if (!vsync) {
            for (size_t i = 0; i < presentModeCount; i++) {
                if (presentModes[i] == VK_PRESENT_MODE_MAILBOX_KHR) {
                    spdlog::debug("selecting mailbox present mode");
                    swapchainPresentMode = VK_PRESENT_MODE_MAILBOX_KHR;
                    break;
                }
                if ((swapchainPresentMode != VK_PRESENT_MODE_MAILBOX_KHR) && (presentModes[i] == VK_PRESENT_MODE_IMMEDIATE_KHR)) {
					swapchainPresentMode = VK_PRESENT_MODE_IMMEDIATE_KHR;
				}
            }
        }

        uint32_t desiredSwapchainImagesCount = surfaceCapabilities.minImageCount + 1;
        if ((surfaceCapabilities.maxImageCount > 0) && (desiredSwapchainImagesCount > surfaceCapabilities.maxImageCount)) {
            desiredSwapchainImagesCount = surfaceCapabilities.maxImageCount;
        }

        VkSurfaceTransformFlagsKHR preTransform;
        if (surfaceCapabilities.supportedTransforms & VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR) {
            preTransform = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;
        } else {
            preTransform = surfaceCapabilities.currentTransform;
        }

        VkCompositeAlphaFlagBitsKHR compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;

        std::vector<VkCompositeAlphaFlagBitsKHR> compositeAlphaFlags = {
			VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
			VK_COMPOSITE_ALPHA_PRE_MULTIPLIED_BIT_KHR,
			VK_COMPOSITE_ALPHA_POST_MULTIPLIED_BIT_KHR,
			VK_COMPOSITE_ALPHA_INHERIT_BIT_KHR,
		};
		for (auto& compositeAlphaFlag : compositeAlphaFlags) {
			if (surfaceCapabilities.supportedCompositeAlpha & compositeAlphaFlag) {
				compositeAlpha = compositeAlphaFlag;
				break;
			};
		}

        VkSwapchainCreateInfoKHR swapchainCI = {};
        swapchainCI.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
        swapchainCI.pNext = nullptr;
        swapchainCI.surface = surface;
        swapchainCI.minImageCount = desiredSwapchainImagesCount;
        // swapchainCI.imageFormat = colorFormat;
        // swapchainCI.imageColorSpace = colorSpace;
    }

    VkSurfaceKHR getSurface() {
        return surface;
    }
};