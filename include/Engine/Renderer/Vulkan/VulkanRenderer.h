#pragma once

#include <vector>
#include <stdexcept>

#include "volk.h"

#include "SDL.h"
#undef main

#include "SDL_vulkan.h"
#include "Engine/Renderer/Renderer.h"

const std::vector<const char*> validationLayers = {
    "VK_LAYER_KHRONOS_validation"
};

#ifdef NDEBUG
    const bool enableValidationLayers = false;
#else
    const bool enableValidationLayers = true;
#endif

class VulkanRenderer {
private:
    void createInstance();
public:
    bool checkValidationLayerSupport();
    void init(const RendererParams &params);
    void beginFrame();
    void endFrame();
};