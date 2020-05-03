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
}

VulkanImage::~VulkanImage() {
  if (image != VK_NULL_HANDLE && allocation != VK_NULL_HANDLE) {
    vmaDestroyImage(device->getAllocator(), image, allocation);
  }
}