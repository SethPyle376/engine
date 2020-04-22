#include "Engine/Renderer/Vulkan/Resources/VulkanMeshResource.h"

VulkanMeshResource::VulkanMeshResource(VulkanDevice *device) {
  this->device = device;
  this->buffer = createBuffer();
}

VulkanMeshResource::~VulkanMeshResource() { delete buffer; }

VulkanBuffer *VulkanMeshResource::createBuffer() {
  VkDeviceSize size = sizeof(vertices[0]) * vertices.size();
  VulkanBuffer stagingBuffer(device, size,
                             VK_BUFFER_USAGE_VERTEX_BUFFER_BIT |
                                 VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
                             VMA_MEMORY_USAGE_CPU_ONLY);
  stagingBuffer.update(reinterpret_cast<const void *>(vertices.data()), size,
                       0);

  VulkanBuffer *gpuBuffer = new VulkanBuffer(
      device, size,
      VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
      VMA_MEMORY_USAGE_GPU_ONLY);

  gpuBuffer->transferDataFrom(&stagingBuffer);

  return gpuBuffer;
}

void VulkanMeshResource::loadVertices() {
  buffer->update(reinterpret_cast<const void *>(vertices.data()),
                 sizeof(Vertex) * vertices.size(), 0);
}

VkBuffer VulkanMeshResource::getBuffer() { buffer->getBuffer(); }

uint32_t VulkanMeshResource::getSize() {
  return static_cast<uint32_t>(vertices.size());
}