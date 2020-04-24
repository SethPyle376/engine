#include "Engine/Renderer/Vulkan/Resources/VulkanMeshInstanceResource.h"

VulkanMeshInstanceResource::VulkanMeshInstanceResource(VulkanDevice* device, int frameCount)
{
  this->device = device;
  this->frameCount = frameCount;
  uniformBuffers.resize(frameCount);
}

VulkanMeshInstanceResource::~VulkanMeshInstanceResource()
{
  for (int i = 0; i < uniformBuffers.size(); i++)
  {
    delete uniformBuffers[i];
  }
  uniformBuffers.clear();
}

void VulkanMeshInstanceResource::createUniformBuffers()
{
  VkDeviceSize size = sizeof(UniformBufferObject);

  for (int i = 0; i < frameCount; i++)
  {
    uniformBuffers[i] = new VulkanBuffer(device, size, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VMA_MEMORY_USAGE_CPU_TO_GPU);
  }
}

void VulkanMeshInstanceResource::update(int imageIndex, UniformBufferObject ubo)
{
  uniformBuffers[imageIndex]->update(reinterpret_cast<void*>(&ubo), sizeof(UniformBufferObject), 0);
}