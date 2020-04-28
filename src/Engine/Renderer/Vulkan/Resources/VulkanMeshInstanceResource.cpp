#include "Engine/Renderer/Vulkan/Resources/VulkanMeshInstanceResource.h"

VulkanMeshInstanceResource::VulkanMeshInstanceResource(VulkanDevice* device, uint32_t descriptorIndex, int frameCount)
{
  
  this->descriptorIndex = descriptorIndex;
  this->device = device;
  this->frameCount = frameCount;
}

VulkanMeshInstanceResource::~VulkanMeshInstanceResource()
{

}

void VulkanMeshInstanceResource::update(int imageIndex, UniformBufferObject ubo)
{

}