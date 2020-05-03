#include "Engine/Renderer/Vulkan/VulkanImage.h"

VulkanImage::VulkanImage(VulkanDevice *device, int width, int height,
                         VkImageUsageFlags imageUsage,
                         VmaMemoryUsage memoryUsage,
                         VmaAllocationCreateFlags flags) {
  this->device = device;

  VkImageCreateInfo imageInfo = {};
  imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
  imageInfo.imageType = VK_IMAGE_TYPE_2D;
  imageInfo.extent.width = static_cast<uint32_t>(width);
  imageInfo.extent.height = static_cast<uint32_t>(height);
  imageInfo.extent.depth = 1;
  imageInfo.mipLevels = 1;
  imageInfo.arrayLayers = 1;
  imageInfo.format = VK_FORMAT_R8G8B8A8_SRGB;
  imageInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
  imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
  imageInfo.usage = imageUsage;
  imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
  imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;

  VmaAllocationCreateInfo memoryInfo{};
  memoryInfo.flags = flags;
  memoryInfo.usage = memoryUsage;

  VmaAllocationInfo allocationInfo = {};

  VkResult result =
      vmaCreateImage(device->getAllocator(), &imageInfo, &memoryInfo, &image,
                     &allocation, &allocationInfo);

  if (result != VK_SUCCESS) {
    spdlog::error("failed to create vulkan image");
  } else {
    memory = allocationInfo.deviceMemory;
  }

  transitionLayout(VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
}

VulkanImage::~VulkanImage() {
  if (image != VK_NULL_HANDLE && allocation != VK_NULL_HANDLE) {
    vmaDestroyImage(device->getAllocator(), image, allocation);
  }
}

void VulkanImage::transitionLayout(VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout) {
  VkCommandBufferAllocateInfo allocInfo = {};
  allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
  allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
  allocInfo.commandPool = device->getCommandPool();
  allocInfo.commandBufferCount = 1;

  VkCommandBuffer commandBuffer;
  vkAllocateCommandBuffers(device->getDevice(), &allocInfo, &commandBuffer);

  VkCommandBufferBeginInfo beginInfo = {};
  beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
  beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

  VkImageMemoryBarrier barrier = {};
  barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
  barrier.oldLayout = oldLayout;
  barrier.newLayout = newLayout;
  barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
  barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
  barrier.image = image;
  barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
  barrier.subresourceRange.baseMipLevel = 0;
  barrier.subresourceRange.levelCount = 1;
  barrier.subresourceRange.baseArrayLayer = 0;
  barrier.subresourceRange.layerCount = 1;

  // TODO
  barrier.srcAccessMask = 0;
  barrier.dstAccessMask = 0;

  vkBeginCommandBuffer(commandBuffer, &beginInfo);

  vkCmdPipelineBarrier(commandBuffer, 0, 0, 0, 0, nullptr, 0, nullptr, 1, &barrier);

  VkSubmitInfo submitInfo = {};
  submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
  submitInfo.commandBufferCount = 1;
  submitInfo.pCommandBuffers = &commandBuffer;

  vkEndCommandBuffer(commandBuffer);

  vkQueueSubmit(device->getGraphicsQueue(), 1, &submitInfo, VK_NULL_HANDLE);
  vkQueueWaitIdle(device->getGraphicsQueue());

  vkFreeCommandBuffers(device->getDevice(), device->getCommandPool(), 1,
                       &commandBuffer);
}