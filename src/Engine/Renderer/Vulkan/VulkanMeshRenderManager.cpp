#include "Engine/Renderer/Vulkan/VulkanMeshRenderManager.h"

VulkanMeshRenderManager::VulkanMeshRenderManager(VulkanDevice *device, int maxObjects) {
  std::shared_ptr<Resource> resource = ResourceManager::getInstance()->getResource("assets/shaders/test_vk_resource.json");
  this->pipeline = std::static_pointer_cast<VulkanPipelineResource>(resource);
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

void VulkanMeshRenderManager::draw(const VulkanRenderFrame& frame) {
  std::vector<std::shared_ptr<Resource>> meshes = ResourceManager::getInstance()->getResources(RESOURCE_VULKAN_MESH_INSTANCE);

  vkCmdBindPipeline(frame.commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline->getPipeline());

  for (int i = 0; i < meshes.size(); i++) {
    std::shared_ptr<VulkanMeshInstanceResource> meshInstance = std::static_pointer_cast<VulkanMeshInstanceResource>(meshes[i]);
    std::shared_ptr<VulkanMeshResource> meshResource = meshInstance->getMesh();

    VkBuffer vertexBuffers[] = {meshResource->getVertexBuffer()};
    VkDeviceSize offsets[] = {0};

    vkCmdBindVertexBuffers(frame.commandBuffer, 0, 1, vertexBuffers, offsets);
    vkCmdBindIndexBuffer(frame.commandBuffer, meshResource->getIndexBuffer(), 0, VK_INDEX_TYPE_UINT16);

    vkCmdDrawIndexed(frame.commandBuffer, static_cast<uint32_t>(meshResource->getIndexCount()), 1, 0, 0, 0);
  }
}