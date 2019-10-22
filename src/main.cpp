#include "Engine/Renderer/Vulkan/VulkanRenderer.h"

int main() {
    RendererParams params;
    VulkanRenderer vulkanRenderer = VulkanRenderer();
    vulkanRenderer.init(params);
    while (true) {
        continue;
    }
}