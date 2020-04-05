#include "Engine/Renderer/Vulkan/VulkanRenderer.h"
#include "Engine/Renderer/Vulkan/Resources/VulkanShaderResource.h"

#include "Engine/Resources/ResourceManager.h"
#include "Engine/Resources/MockResourceFactory.h"
#include "Engine/Renderer/Vulkan/Resources/VulkanShaderResourceFactory.h"

int main() {
	ResourceFactory* mockFactory = new MockResourceFactory();
	VulkanShaderResourceFactory* vulkanFactory = new VulkanShaderResourceFactory();

	ResourceManager resourceManager;
	resourceManager.registerFactory(mockFactory);
	resourceManager.registerFactory(vulkanFactory);
	
    RendererParams params;
    params.x = 1080;
    params.y = 720;
    VulkanRenderer vulkanRenderer = VulkanRenderer(params);
    vulkanRenderer.init();
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