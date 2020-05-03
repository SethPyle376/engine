#include "Engine/Renderer/Vulkan/Resources/VulkanPipelineResource.h"
#include "Engine/Renderer/Vulkan/VulkanRenderer.h"
#include "Engine/Renderer/Vulkan/VulkanDynamicBuffer.h"

#include "Engine/Scene/Actor.h"
#include "Engine/Scene/Node.h"
#include "Engine/Scene/Scene.h"

#include "Engine/Renderer/Vulkan/Resources/VulkanMeshResourceFactory.h"
#include "Engine/Renderer/Vulkan/Resources/VulkanPipelineResourceFactory.h"
#include "Engine/Resources/MockResourceFactory.h"
#include "Engine/Resources/ResourceManager.h"

#include "Engine/Renderer/Vulkan/Resources/VulkanMeshInstanceResourceFactory.h"
#include "Engine/Renderer/Vulkan/Resources/VulkanMeshInstanceResource.h"
#include "Engine/Renderer/Vulkan/VulkanMeshRenderManager.h"
#include "Engine/Renderer/Vulkan/VulkanImage.h"

int main() {
  spdlog::set_level(spdlog::level::debug);

  ResourceFactory *mockFactory = new MockResourceFactory();
  ResourceManager *resourceManager = ResourceManager::getInstance();

  RendererParams params;
  params.x = 1920;
  params.y = 1080;
  VulkanRenderer vulkanRenderer = VulkanRenderer(params);
  vulkanRenderer.init();

  VulkanPipelineResourceFactory *vulkanPipelineFactory =
      new VulkanPipelineResourceFactory(vulkanRenderer.getDevice(), params,
                                        vulkanRenderer.getRenderPass());
  resourceManager->registerFactory(vulkanPipelineFactory);

  VulkanMeshResourceFactory *vulkanMeshFactory =
      new VulkanMeshResourceFactory(vulkanRenderer.getDevice());
  resourceManager->registerFactory(vulkanMeshFactory);

  VulkanMeshInstanceResourceFactory *vulkanMeshInstanceFactory = new VulkanMeshInstanceResourceFactory(vulkanRenderer.getDevice());
  resourceManager->registerFactory(vulkanMeshInstanceFactory);

  bool quit = false;
  SDL_Event e;

  std::shared_ptr<Resource> vulkanShaderResource =
      resourceManager->getResource("assets/shaders/test_vk_resource.json");

  std::shared_ptr<Resource> vulkanMeshResource =
      resourceManager->getResource("assets/meshes/test_vk_mesh.json");

  std::shared_ptr<Resource> vulkanMeshInstanceResource = resourceManager->getResource("assets/meshes/test_vk_mesh_instance.json");

  std::shared_ptr<VulkanMeshInstanceResource> meshInstance = std::static_pointer_cast<VulkanMeshInstanceResource>(vulkanMeshInstanceResource);

  VulkanMeshRenderManager meshRenderManager = VulkanMeshRenderManager(vulkanRenderer.getDevice(), 100, vulkanRenderer.getFrameCount());

  VulkanImage image = VulkanImage(vulkanRenderer.getDevice(), 256, 256, VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, VMA_MEMORY_USAGE_GPU_ONLY);

  Actor* actor = new Actor(meshInstance);
  Scene scene = Scene();
  scene.addNode(actor);

  while (!quit) {
    scene.update();
    std::vector<Node*> drawList = scene.getMeshDrawList();
    // Render stuff
    VulkanRenderFrame frame = vulkanRenderer.prepareFrame();
    frame.begin();
    meshRenderManager.draw(frame, drawList);
    frame.end();
    vulkanRenderer.submitFrame(frame);

    // SDL input stuff
    SDL_PollEvent(&e);
    if (e.type == SDL_QUIT) {
      quit = true;
    }
  }
  vulkanRenderer.finishFrame();

  return 0;
}