#include "Engine/Renderer/Vulkan/Resources/VulkanPipelineResource.h"

VkShaderModule VulkanPipelineResource::createShaderModule(const std::vector<char> &code) {
    VkShaderModuleCreateInfo createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    createInfo.codeSize = code.size();
    createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());

    VkShaderModule shaderModule;
    if (vkCreateShaderModule(device, &createInfo, nullptr, &shaderModule) != VK_SUCCESS) {
        spdlog::error("failed to compile shader module");
    } else {
        spdlog::debug("compiled shader code");
    }
    return shaderModule;
}

VulkanPipelineResource::~VulkanPipelineResource() {
    for (int i = 0; i < createdModules.size(); i++) {
        vkDestroyShaderModule(device, createdModules[i], nullptr);
    }
}

void VulkanPipelineResource::load(const std::vector<char> &vertexCode, const std::vector<char> &fragmentCode) {
    VkShaderModule vertModule = createShaderModule(vertexCode);
    VkShaderModule fragModule = createShaderModule(fragmentCode);

    createdModules.push_back(vertModule);
    createdModules.push_back(fragModule);
}