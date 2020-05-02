#pragma once

#include <vector>

#include "Engine/Resources/Resource.h"

#include "Engine/Renderer/Vulkan/VulkanBuffer.h"
#include "Engine/Renderer/Vulkan/VulkanDevice.h"
#include "Engine/Renderer/Vulkan/VulkanVertex.h"

const std::vector<Vertex> vertices = {{{-0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}},
                                      {{0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}},
                                      {{0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}},
                                      {{-0.5f, 0.5f}, {1.0f, 1.0f, 1.0f}}};

const std::vector<uint16_t> indices = {0, 1, 2, 2, 3, 0};

class VulkanMeshResource : public Resource {
private:
  VulkanDevice *device;

  VulkanBuffer *vertexBuffer;
  VulkanBuffer *indexBuffer;

  void loadBuffers();

public:
  VulkanMeshResource(VulkanDevice *device);
  ~VulkanMeshResource();

  VkBuffer getVertexBuffer();
  VkBuffer getIndexBuffer();

  int getIndexCount();
};
