#pragma once

#include "SDL_vulkan.h"
#include "spdlog/spdlog.h"

typedef struct _SwapChainBuffers {
	VkImage image;
	VkImageView view;
} SwapChainBuffer;

class VulkanSwapchain {
private:
    VkInstance instance;
    VkPhysicalDevice physicalDevice;
    VkDevice device;
    VkSurfaceKHR surface;

    VkSwapchainKHR swapchain = VK_NULL_HANDLE;
    std::vector<SwapChainBuffer> buffers;
    std::vector<VkImage> images;

    VkFormat colorFormat;
    VkColorSpaceKHR colorSpace;

    uint32_t imageCount;

public:
    VulkanSwapchain(VkInstance instance) {
        this->instance = instance;
    }
    
    ~VulkanSwapchain() {
        cleanup();
    }

    void cleanup() {
        for (uint32_t i = 0; i < imageCount; i++) {
            vkDestroyImageView(device, buffers[i].view, nullptr);
        }
        vkDestroySwapchainKHR(device, swapchain, nullptr);
        vkDestroySurfaceKHR(instance, surface, nullptr);
        surface = VK_NULL_HANDLE;
		swapchain = VK_NULL_HANDLE;
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

    void selectColors() {
        uint32_t formatCount;
        vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &formatCount, nullptr);

        if (formatCount <= 0) {
            spdlog::error("no vulkan color formats found");
        }

        std::vector<VkSurfaceFormatKHR> surfaceFormats(formatCount);
        vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &formatCount, surfaceFormats.data());

        //In this case there is no preferred format
        if ((formatCount == 1) && (surfaceFormats[0].format == VK_FORMAT_UNDEFINED)) {
            colorFormat = VK_FORMAT_B8G8R8A8_UNORM;
            colorSpace = surfaceFormats[0].colorSpace;
        } else {
            bool foundPreferred = false;
            for (auto&& surfaceFormat : surfaceFormats) {
                if (surfaceFormat.format == VK_FORMAT_B8G8R8A8_UNORM) {
                    colorFormat = surfaceFormat.format;
                    colorSpace = surfaceFormat.colorSpace;
                    foundPreferred = true;
                    break;
                }
            }

            if (!foundPreferred) {
                colorFormat = surfaceFormats[0].format;
                colorSpace = surfaceFormats[0].colorSpace;
            }
        }
    }

    void create(uint32_t width, uint32_t height, bool vsync = false) {
        selectColors();
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

        spdlog::debug("Present Mode Count: {0}", presentModeCount);

        if (!vsync) {
            for (size_t i = 0; i < presentModeCount; i++) {
                if (presentModes[i] == VK_PRESENT_MODE_MAILBOX_KHR) {
                    spdlog::debug("selecting mailbox present mode");
                    swapchainPresentMode = VK_PRESENT_MODE_MAILBOX_KHR;
                    break;
                }
                if ((swapchainPresentMode != VK_PRESENT_MODE_MAILBOX_KHR) && (presentModes[i] == VK_PRESENT_MODE_IMMEDIATE_KHR)) {
                    spdlog::debug("selecting immediate present mode");
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
        swapchainCI.imageFormat = colorFormat;
        swapchainCI.imageColorSpace = colorSpace;
        swapchainCI.imageExtent = { swapchainExtent.width, swapchainExtent.height };
        swapchainCI.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
        swapchainCI.preTransform = (VkSurfaceTransformFlagBitsKHR)preTransform;
        swapchainCI.imageArrayLayers = 1;
        swapchainCI.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
        swapchainCI.queueFamilyIndexCount = 0;
        swapchainCI.queueFamilyIndexCount = 0;
        swapchainCI.pQueueFamilyIndices = nullptr;
        swapchainCI.presentMode = swapchainPresentMode;
        swapchainCI.oldSwapchain = oldSwapchain;
        swapchainCI.clipped = VK_TRUE;
        swapchainCI.compositeAlpha = compositeAlpha;

        if (surfaceCapabilities.supportedUsageFlags & VK_IMAGE_USAGE_TRANSFER_SRC_BIT) {
            swapchainCI.imageUsage |= VK_IMAGE_USAGE_TRANSFER_SRC_BIT;
        }

        if (surfaceCapabilities.supportedUsageFlags & VK_IMAGE_USAGE_TRANSFER_DST_BIT) {
            swapchainCI.imageUsage |= VK_IMAGE_USAGE_TRANSFER_DST_BIT;
        }

        VkResult result = vkCreateSwapchainKHR(device, &swapchainCI, nullptr, &swapchain);

        if (result != VK_SUCCESS) {
            spdlog::error("vulkan failed to create swapchain");
        } else {
            spdlog::debug("vulkan successfully created swapchain");
        }

        if (oldSwapchain != VK_NULL_HANDLE) {
            for (uint32_t i = 0; i < imageCount; i++) {
                vkDestroyImageView(device, buffers[i].view, nullptr);
            }
            vkDestroySwapchainKHR(device, oldSwapchain, nullptr);
        }
        vkGetSwapchainImagesKHR(device, swapchain, &imageCount, nullptr);

        images.resize(imageCount);
        vkGetSwapchainImagesKHR(device, swapchain, &imageCount, images.data());

        buffers.resize(imageCount);
        for (uint32_t i = 0; i < imageCount; i++) {
            VkImageViewCreateInfo colorAttachmentView = {};
            colorAttachmentView.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
            colorAttachmentView.pNext = nullptr;
            colorAttachmentView.format = colorFormat;
            colorAttachmentView.components = {
                VK_COMPONENT_SWIZZLE_R,
                VK_COMPONENT_SWIZZLE_G,
                VK_COMPONENT_SWIZZLE_B,
                VK_COMPONENT_SWIZZLE_A
            };
            colorAttachmentView.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
            colorAttachmentView.subresourceRange.baseMipLevel = 0;
            colorAttachmentView.subresourceRange.levelCount = 1;
            colorAttachmentView.subresourceRange.baseArrayLayer = 0;
            colorAttachmentView.subresourceRange.layerCount = 1;
            colorAttachmentView.viewType = VK_IMAGE_VIEW_TYPE_2D;
            colorAttachmentView.flags = 0;

            buffers[i].image = images[i];

            colorAttachmentView.image = buffers[i].image;
            VkResult result = vkCreateImageView(device, &colorAttachmentView, nullptr, &buffers[i].view);

            if (result != VK_SUCCESS) {
                spdlog::error("vulkan failed to create image view");
            } else {
                spdlog::debug("vulkan created image view");
            }
        }
    }

    VkSurfaceKHR getSurface() {
        return surface;
    }
};