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
  int frameCount = 0;
  VulkanDevice* device;
  UniformBufferObject ubo;

  std::shared_ptr<VulkanMeshResource> mesh;

  std::vector<VulkanBuffer*> uniformBuffers;

  void createUniformBuffers();
public:
  VulkanMeshInstanceResource(VulkanDevice* device, int frameCount);
  ~VulkanMeshInstanceResource();

  void update(int imageIndex, UniformBufferObject ubo);
};