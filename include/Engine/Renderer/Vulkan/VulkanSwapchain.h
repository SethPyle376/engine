#pragma once

#include "SDL_vulkan.h"
#include "spdlog/spdlog.h"

class VulkanSwapchain {
private:
    VkInstance instance;
    VkSurfaceKHR surface;
public:
    VulkanSwapchain(VkInstance instance) {
        this->instance = instance;
    }
    
    ~VulkanSwapchain() {
        vkDestroySurfaceKHR(instance, surface, nullptr);
    }

    void initSurface(SDL_Window *window) {
        if (!SDL_Vulkan_CreateSurface(window, instance, &surface)) {
            spdlog::error("failed to create vulkan surface");
        } else {
            spdlog::debug("created vulkan surface");
        }
    }

    VkSurfaceKHR getSurface() {
        return surface;
    }
};