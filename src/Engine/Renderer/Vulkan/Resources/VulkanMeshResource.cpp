#include "Engine/Renderer/Vulkan/Resources/VulkanMeshResource.h"

VulkanMeshResource::VulkanMeshResource(VulkanDevice* device) {
    this->device = device;
    this->buffer = createBuffer();
    loadVertices();
}

VulkanBuffer* VulkanMeshResource::createBuffer() {
    return new VulkanBuffer(device, sizeof(vertices[0]) * vertices.size(), VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VMA_MEMORY_USAGE_CPU_TO_GPU);
}

void VulkanMeshResource::loadVertices() {
    buffer->update(reinterpret_cast<const void*>(vertices.data()), sizeof(Vertex) * vertices.size(), 0);
}

VkBuffer VulkanMeshResource::getBuffer() {
    buffer->getBuffer();
}

VulkanMeshResource::~VulkanMeshResource() {
    delete buffer;
}

uint32_t VulkanMeshResource::getSize() {
    return static_cast<uint32_t>(vertices.size());
}