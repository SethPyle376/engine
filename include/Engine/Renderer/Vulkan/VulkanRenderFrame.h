#pragma once

#include "volk.h"
#include "Engine/Renderer/RenderFrame.h"

class VulkanRenderFrame {
public:
  VkCommandBuffer commandBuffer;
  int currentFrameIndex;
  uint32_t currentImageIndex;
};