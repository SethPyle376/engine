#pragma once

#include "Engine/Renderer/Renderer.h"
#include "Engine/Resources/Resource.h"
#include "SDL.h"
#include "spdlog/spdlog.h"
#include "volk.h"

#include "Engine/Renderer/Vulkan/VulkanDevice.h"
#include "Engine/Renderer/Vulkan/VulkanVertex.h"

class VulkanPipelineResource : public Resource {
private:
  VkShaderModule createShaderModule(const std::vector<char> &code);

  VulkanDevice *device;
  RendererParams params;
  std::vector<VkShaderModule> createdModules;

  VkRenderPass renderPass;
  VkPipelineLayout pipelineLayout;
  VkPipeline graphicsPipeline;

public:
  VulkanPipelineResource(VulkanDevice *device, RendererParams params,
                         VkRenderPass renderPass,
                         const std::vector<char> &vertexCode,
                         const std::vector<char> &fragmentCode) {
    this->params = params;
    this->device = device;
    this->renderPass = renderPass;
    load(vertexCode, fragmentCode);
  };

  ~VulkanPipelineResource();

  void load(const std::vector<char> &vertexCode,
            const std::vector<char> &fragmentCode);

  VkPipeline getPipeline();
};