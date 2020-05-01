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

#include "Engine/Renderer/Vulkan/Resources/VulkanMeshInstanceResource.h"
#include "Engine/Renderer/Vulkan/VulkanMeshRenderManager.h"

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

  bool quit = false;
  SDL_Event e;

  std::shared_ptr<Resource> vulkanShaderResource =
      resourceManager->getResource("assets/shaders/test_vk_resource.json");

  std::shared_ptr<Resource> vulkanMeshResource =
      resourceManager->getResource("assets/meshes/test_vk_mesh.json");

  VulkanMeshRenderManager meshRenderManager = VulkanMeshRenderManager(vulkanRenderer.getDevice(), 1000);

  vulkanRenderer.beginFrame();
  while (!quit) {

    // Render stuff
    VulkanRenderFrame frame = vulkanRenderer.prepareFrame();
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