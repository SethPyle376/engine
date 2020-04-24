#pragma once

#include "glm/glm.hpp"

struct UniformBufferObject {
  glm::mat4 model;
  glm::mat4 view;
  glm::mat4 projection;
};

class VulkanMeshInstanceResource {
private:
  UniformBufferObject ubo;
public:
};