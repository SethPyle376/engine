#include "Engine/Renderer/Vulkan/VulkanRenderer.h"
#include "Engine/Renderer/Vulkan/Resources/VulkanShaderResource.h"

#include "Engine/Resources/ResourceManager.h"
#include "Engine/Resources/MockResourceFactory.h"

void testFunction(ResourceManager& resourceManager) {
	std::shared_ptr<Resource> resource = resourceManager.getResource("assets/test.json");
	std::shared_ptr<MockResource> mockResource = std::static_pointer_cast<MockResource>(resource);

	spdlog::info(mockResource.get()->payload);
}

int main() {
	ResourceFactory* mockFactory = new MockResourceFactory();

	ResourceManager resourceManager;
	resourceManager.registerFactory(mockFactory);
	
    RendererParams params;
    params.x = 1080;
    params.y = 720;
    VulkanRenderer vulkanRenderer = VulkanRenderer(params);
    vulkanRenderer.init();
	VulkanShaderResource test("test");
	test.load();
    bool quit = false;
	SDL_Event e;

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