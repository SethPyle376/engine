#pragma once

#include <vector>
#include <stdexcept>

#include "volk.h"

#include "SDL.h"
#undef main
#include "SDL_vulkan.h"

#include "Engine/Renderer/Renderer.h"

#include "Engine/Renderer/Vulkan/VulkanUtils.h"
#include "Engine/Renderer/Vulkan/VulkanDevice.h"

const std::vector<const char*> validationLayers = {
    "VK_LAYER_LUNARG_standard_validation"
};

const std::vector<const char*> deviceExtensions = {
    VK_KHR_SWAPCHAIN_EXTENSION_NAME
};

class VulkanRenderer {
private:
    #ifdef NDEBUG
        const bool enableValidationLayers = false;
        std::vector<const char*> extensions;
    #else
        const bool enableValidationLayers = true;
        std::vector<const char*> extensions = {
            VK_EXT_DEBUG_REPORT_EXTENSION_NAME
        };
    #endif
    RendererParams params;
    SDL_Window* window;

    VkInstance instance;
    VkSurfaceKHR surface;
    VkPhysicalDevice physicalDevice;
    VkDevice device;

    void initSDL();
    void initWindow();
    void initVolk();
    void createInstance();
    void initSurface();
    void pickPhysicalDevice();
    void initLogicalDevice();
public:
    VulkanRenderer(const RendererParams &params);
    ~VulkanRenderer();
    bool checkValidationLayerSupport();
    void init();
    void beginFrame();
    void endFrame();
};