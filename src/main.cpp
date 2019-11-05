#include "Engine/Renderer/Vulkan/VulkanRenderer.h"

int main() {
    RendererParams params;
    params.x = 1080;
    params.y = 720;
    VulkanRenderer vulkanRenderer = VulkanRenderer(params);
    vulkanRenderer.init();
    while (true) {
        continue;
    }
}