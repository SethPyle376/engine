#pragma once

#include "Engine/Scene/Node.h"

#include "Engine/Renderer/Vulkan/Resources/VulkanMeshInstanceResource.h"

#include <glm/gtc/matrix_transform.hpp>

#include <chrono>

class Actor : public Node {
private:
  std::shared_ptr<VulkanMeshInstanceResource> meshInstance;
public:
  Actor(std::shared_ptr<VulkanMeshInstanceResource> meshInstance);
  ~Actor();

  std::shared_ptr<VulkanMeshInstanceResource> getMeshInstance();

  void update() override;
};