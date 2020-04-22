#pragma once

#include "Engine/Resources/ResourceFactory.h"

#include "Engine/Renderer/Vulkan/Resources/VulkanMeshResource.h"
#include "Engine/Renderer/Vulkan/VulkanDevice.h"

class VulkanMeshResourceFactory : public ResourceFactory {
private:
  VulkanDevice *device;

public:
  VulkanMeshResourceFactory(VulkanDevice *device) {
    this->device = device;
    this->resourceType = "vulkan_mesh";
  }

  std::shared_ptr<Resource> load(const std::string &path) {
    std::shared_ptr<VulkanMeshResource> ptr(new VulkanMeshResource(device));
    ptr->setResourceType(RESOURCE_VULKAN_MESH);
    return std::static_pointer_cast<Resource>(ptr);
  }
};