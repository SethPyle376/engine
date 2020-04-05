#pragma once

#include "SDL.h"
#include "spdlog/spdlog.h"
#include "volk.h"
#include "Engine/Resources/Resource.h"

class VulkanShaderResource : public Resource {
private:
    VkShaderModule createShaderModule(const char* code);

public:
    VulkanShaderResource(const char* vertexCode, const char* fragmentCode) {
        load(vertexCode, fragmentCode);
    };

    void load(const char* vertexCode, const char* fragmentCode);
};