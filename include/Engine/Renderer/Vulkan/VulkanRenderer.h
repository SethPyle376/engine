#pragma once

#include <vector>
#include <stdexcept>

#include "volk.h"

#include "SDL2/SDL.h"
#undef main

#include "SDL2/SDL_vulkan.h"
#include "Engine/Renderer/Renderer.h"

const std::vector<const char*> validationLayers = {
    "VK_LAYER_KHRONOS_validation"
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

    void initSDL();
    void initWindow();
    void initVolk();
    void createInstance();
public:
    VulkanRenderer(const RendererParams &params);
    ~VulkanRenderer();
    bool checkValidationLayerSupport();
    void init();
    void beginFrame();
    void endFrame();
};