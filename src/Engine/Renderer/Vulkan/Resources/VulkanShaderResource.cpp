#include "Engine/Renderer/Vulkan/Resources/VulkanShaderResource.h"

void VulkanShaderResource::load(const char* vertexCode, const char* fragmentCode) {
    spdlog::debug("Loaded vertex code: {0} and fragment code: {1}", vertexCode, fragmentCode);
}