#pragma once

#include "volk.h"
#include "vk_mem_alloc.h"

#include "Engine/Renderer/Vulkan/VulkanDevice.h"
#include "Engine/Renderer/Vulkan/VulkanBuffer.h"

class VulkanImage {
private:
  VulkanDevice *device;
  VkImage image;
  VmaAllocation allocation = VK_NULL_HANDLE;
  VkDeviceMemory memory;

  void transitionLayout(VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout);

public:
  VulkanImage(
      VulkanDevice *device, int width, int height, VkImageUsageFlags imageUsage,
      VmaMemoryUsage memoryUsage,
      VmaAllocationCreateFlags flags = VMA_ALLOCATION_CREATE_MAPPED_BIT);
  ~VulkanImage();

  void transferFromBuffer(VulkanBuffer* buffer);
};