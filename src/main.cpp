#include "Engine/Renderer/Vulkan/VulkanRenderer.h"
#include "Engine/Renderer/Vulkan/Resources/VulkanPipelineResource.h"

#include "Engine/Resources/ResourceManager.h"
#include "Engine/Resources/MockResourceFactory.h"
#include "Engine/Renderer/Vulkan/Resources/VulkanPipelineResourceFactory.h"

int main() {
	ResourceFactory* mockFactory = new MockResourceFactory();


	ResourceManager resourceManager;
	resourceManager.registerFactory(mockFactory);
	
    RendererParams params;
    params.x = 1080;
    params.y = 720;
    VulkanRenderer vulkanRenderer = VulkanRenderer(params);
    vulkanRenderer.init();

	VulkanPipelineResourceFactory* vulkanFactory = new VulkanPipelineResourceFactory(vulkanRenderer.getLogicalDevice(), params);
	resourceManager.registerFactory(vulkanFactory);

    bool quit = false;
	SDL_Event e;

	std::shared_ptr<Resource> vulkanShaderResource = resourceManager.getResource("assets/shaders/test_vk_resource.json");

	while (!quit) {
		vulkanRenderer.beginFrame();
		vulkanRenderer.endFrame();
		SDL_PollEvent(&e);
		if (e.type == SDL_QUIT) {
			quit = true;
		}
	}
	return 0;
}