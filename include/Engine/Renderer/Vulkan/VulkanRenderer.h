#pragma once

#include "SDL.h"
#undef main

#include "SDL_vulkan.h"
#include "Engine/Renderer/Renderer.h"

class VulkanRenderer {
public:
    void init(const RendererParams &params);
    void beginFrame();
    void endFrame();
};