#include "Engine/Renderer/Vulkan/Resources/VulkanMeshResource.h"

VulkanMeshResource::VulkanMeshResource(VulkanDevice* device) {
    this->device = device;
    this->buffer = createBuffer();
}

VulkanBuffer* VulkanMeshResource::createBuffer() {
    return new VulkanBuffer(device, sizeof(vertices[0]) * vertices.size(), VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VMA_MEMORY_USAGE_CPU_TO_GPU);
}

VulkanMeshResource::~VulkanMeshResource() {
    delete buffer;
}