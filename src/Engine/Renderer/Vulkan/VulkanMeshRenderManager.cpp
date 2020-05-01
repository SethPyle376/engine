#include "Engine/Renderer/Vulkan/VulkanMeshRenderManager.h"

VulkanMeshRenderManager::VulkanMeshRenderManager(VulkanDevice *device, int maxObjects) {
  this->device = device;
  this->maxObjects = maxObjects;
  initBuffers();
}

VulkanMeshRenderManager::~VulkanMeshRenderManager() {
  for (int i = 0; i < uniformBuffers.size(); i++) {
    delete uniformBuffers[i];
  }
  uniformBuffers.clear();
}

void VulkanMeshRenderManager::initBuffers() {
  uniformBuffers.resize(MAX_FRAMES_IN_FLIGHT);

  for (int i = 0; i < uniformBuffers.size(); i++) {
    uniformBuffers[i] = new VulkanDynamicBuffer<glm::mat4>(device, maxObjects);
  }
}

void VulkanMeshRenderManager::draw(VulkanRenderFrame frame) {
  return;
}