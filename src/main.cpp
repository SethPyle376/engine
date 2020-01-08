#include "Engine/Renderer/Vulkan/VulkanRenderer.h"
#include "Engine/Renderer/Vulkan/Resources/VulkanShaderResource.h"

int main() {
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