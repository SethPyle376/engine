#pragma once

#include "Engine/Renderer/RenderManager.h"

#include "Engine/Renderer/Vulkan/VulkanRenderer.h"
#include "Engine/Renderer/Vulkan/Resources/VulkanMeshInstanceResource.h"
#include "Engine/Renderer/Vulkan/Resources/VulkanPipelineResource.h"
#include "Engine/Renderer/Vulkan/VulkanDynamicBuffer.h"
#include "Engine/Renderer/Vulkan/VulkanRenderFrame.h"


class VulkanMeshRenderManager : public RenderManager
{
private:
  VulkanDevice *device;
  std::shared_ptr<VulkanPipelineResource> pipeline;
  std::vector<VulkanDynamicBuffer<glm::mat4>*> uniformBuffers;
  std::vector<VkDescriptorPool> descriptorPool;
  std::vector<VkDescriptorSet> descriptorSets;
  int maxObjects = 0;

  void initBuffers();
public:
  VulkanMeshRenderManager(VulkanDevice *device, int maxObjects);
  ~VulkanMeshRenderManager();

  void draw(const VulkanRenderFrame& frame);
};