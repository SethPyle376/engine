#pragma once

#include "SDL.h"
#include "spdlog/spdlog.h"

#include "Engine/Resources/Resource.h"

class VulkanShaderResource : public Resource {
private:

public:
    VulkanShaderResource::VulkanShaderResource(std::string filename) : Resource(filename) {};

    void load();
};