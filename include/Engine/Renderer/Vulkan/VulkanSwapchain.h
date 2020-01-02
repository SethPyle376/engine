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
    }

    VkSurfaceKHR getSurface() {
        return surface;
    }
};