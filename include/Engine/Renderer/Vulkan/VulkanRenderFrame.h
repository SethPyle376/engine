#pragma once

#include "volk.h"
#include "Engine/Renderer/RenderFrame.h"

class VulkanRenderFrame {
public:
  VkCommandBuffer commandBuffer;
  int currentFrameIndex;
  uint32_t currentImageIndex;
  VkRenderPassBeginInfo renderPassBeginInfo;

  VkViewport viewport;
  VkRect2D scissor;

  VkCommandBufferBeginInfo beginInfo;

  void begin() {
    if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS) {
      spdlog::error("failed to begin vulkan command buffer");
    }

    vkCmdSetViewport(commandBuffer, 0, 1, &viewport);
    vkCmdSetScissor(commandBuffer, 0, 1, &scissor);

    vkCmdBeginRenderPass(commandBuffer, &renderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);
  }

  void end() {
    vkCmdEndRenderPass(commandBuffer);
    vkEndCommandBuffer(commandBuffer);
  }
};