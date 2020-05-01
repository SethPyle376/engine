#pragma once

#include "Engine/Resources/ResourceFactory.h"
#include "Engine/Resources/ResourceManager.h"

#include "Engine/Renderer/Vulkan/Resources/VulkanMeshResource.h"
#include "Engine/Renderer/Vulkan/Resources/VulkanMeshInstanceResource.h"


class VulkanMeshInstanceResourceFactory : public ResourceFactory {
private:
  VulkanDevice *device;
  int currentId = 0;
public:
  VulkanMeshInstanceResourceFactory(VulkanDevice *device) {
    this->device = device;
    this->resourceType = "vulkan_mesh_instance";
  }

  std::shared_ptr<Resource> load(const std::string &path) {
    std::shared_ptr<Resource> mesh = ResourceManager::getInstance()->getResource("assets/meshes/test_vk_mesh.json");
    std::shared_ptr<VulkanMeshResource> meshResource = std::static_pointer_cast<VulkanMeshResource>(mesh);
    std::shared_ptr<VulkanMeshInstanceResource> ptr(new VulkanMeshInstanceResource(device, currentId, meshResource));
    ptr->setResourceType(RESOURCE_VULKAN_MESH_INSTANCE);

    currentId++;
    return std::static_pointer_cast<Resource>(ptr);
  }
};
