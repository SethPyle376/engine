#pragma once

#include <vector>

#include "Engine/Resources/Resource.h"

#include "Engine/Renderer/Vulkan/VulkanDevice.h"
#include "Engine/Renderer/Vulkan/VulkanBuffer.h"
#include "Engine/Renderer/Vulkan/VulkanVertex.h"

const std::vector<Vertex> vertices = {
    {{0.0f, -0.5f}, {1.0f, 0.0f, 0.0f}},
    {{0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}},
    {{-0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}}
};

class VulkanMeshResource : public Resource {
private:
    VulkanBuffer* buffer;
    VulkanBuffer* createBuffer();

    VulkanDevice* device;

public:
    VulkanMeshResource(VulkanDevice* device);
    ~VulkanMeshResource();
};
