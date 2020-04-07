#pragma once

#include "SDL.h"
#include "spdlog/spdlog.h"
#include "volk.h"
#include "Engine/Resources/Resource.h"

class VulkanPipelineResource : public Resource {
private:
    VkShaderModule createShaderModule(const std::vector<char> &code);

    VkDevice device;
    std::vector<VkShaderModule> createdModules;

public:
    VulkanPipelineResource(VkDevice device, const std::vector<char> &vertexCode, const std::vector<char> &fragmentCode) {
        this->device = device;
        load(vertexCode, fragmentCode);
    };

    ~VulkanPipelineResource();

    void load(const std::vector<char> &vertexCode, const std::vector<char> &fragmentCode);
};