#include "Engine/Renderer/Vulkan/Resources/VulkanMeshResource.h"

VulkanMeshResource::VulkanMeshResource(VulkanDevice *device) {
  this->device = device;
  loadBuffers();
}

VulkanMeshResource::~VulkanMeshResource() 
{ 
  delete vertexBuffer;
  delete indexBuffer;
}

void VulkanMeshResource::loadBuffers() {
  VkDeviceSize vertexBufferSize = sizeof(vertices[0]) * vertices.size();
  VkDeviceSize indexBufferSize = sizeof(indices[0]) * indices.size();

  VulkanBuffer vertexStagingBuffer(device, vertexBufferSize,
                                 VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
                             VMA_MEMORY_USAGE_CPU_ONLY);
  vertexStagingBuffer.update(reinterpret_cast<const void *>(vertices.data()), vertexBufferSize,
                       0);

  VulkanBuffer *vertexDeviceBuffer = new VulkanBuffer(
      device, vertexBufferSize,
      VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
      VMA_MEMORY_USAGE_GPU_ONLY);

  vertexDeviceBuffer->transferDataFrom(&vertexStagingBuffer);

  this->vertexBuffer = vertexDeviceBuffer;

  VulkanBuffer indexStagingBuffer(device, indexBufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VMA_MEMORY_USAGE_CPU_ONLY);
  indexStagingBuffer.update(reinterpret_cast<const void *>(indices.data()), indexBufferSize, 0);

  VulkanBuffer* indexDeviceBuffer = new VulkanBuffer(
    device, indexBufferSize,
    VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
    VMA_MEMORY_USAGE_GPU_ONLY);
  
  indexDeviceBuffer->transferDataFrom(&indexStagingBuffer);

  this->indexBuffer = indexDeviceBuffer;
}

VkBuffer VulkanMeshResource::getVertexBuffer() { return vertexBuffer->getBuffer(); }

VkBuffer VulkanMeshResource::getIndexBuffer() { return indexBuffer->getBuffer(); }

int VulkanMeshResource::getIndexCount()
{
  return indices.size();
}