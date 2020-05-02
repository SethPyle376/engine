#include "Engine/Renderer/Vulkan/Resources/VulkanMeshInstanceResource.h"

VulkanMeshInstanceResource::VulkanMeshInstanceResource(VulkanDevice* device, int descriptorIndex, std::shared_ptr<VulkanMeshResource> mesh)
{
  this->mesh = mesh;
  this->descriptorIndex = descriptorIndex;
  this->device = device;
}

VulkanMeshInstanceResource::~VulkanMeshInstanceResource()
{

}

void VulkanMeshInstanceResource::update(UniformBufferObject ubo)
{

}

std::shared_ptr<VulkanMeshResource> VulkanMeshInstanceResource::getMesh() {
  return mesh;
}