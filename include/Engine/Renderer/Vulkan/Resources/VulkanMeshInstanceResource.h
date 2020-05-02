#pragma once

#include "Engine/Renderer/Vulkan/VulkanBuffer.h"
#include "Engine/Renderer/Vulkan/Resources/VulkanMeshResource.h"
#include "Engine/Resources/Resource.h"
#include "glm/glm.hpp"

struct UniformBufferObject {
  glm::mat4 model;
  glm::mat4 view;
  glm::mat4 projection;
};

class VulkanMeshInstanceResource : public Resource {
private:
  int descriptorIndex;
  VulkanDevice* device;
  UniformBufferObject ubo;

  std::shared_ptr<VulkanMeshResource> mesh;
public:
  VulkanMeshInstanceResource(VulkanDevice* device, int descriptorIndex, std::shared_ptr<VulkanMeshResource> mesh);
  ~VulkanMeshInstanceResource();

  std::shared_ptr<VulkanMeshResource> getMesh();

  void update(UniformBufferObject ubo);
};