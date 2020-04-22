#pragma once

#include <string>

#include "Engine/Renderer/Vulkan/Resources/VulkanPipelineResource.h"
#include "Engine/Resources/ResourceFactory.h"

class VulkanPipelineResourceFactory : public ResourceFactory {
private:
  VulkanDevice *device;
  VkRenderPass renderPass;
  RendererParams params;

  std::vector<char> readSpirv(const std::string &path) {
    SDL_RWops *sdlFile = SDL_RWFromFile(path.c_str(), "rb");

    int size = SDL_RWsize(sdlFile);
    std::vector<char> buffer(size);

    SDL_RWread(sdlFile, buffer.data(), size, 1);
    SDL_RWclose(sdlFile);

    return buffer;
  }

public:
  VulkanPipelineResourceFactory(VulkanDevice *device, RendererParams params,
                                VkRenderPass renderPass) {
    this->device = device;
    this->resourceType = "vulkan_shader";
    this->params = params;
    this->renderPass = renderPass;
  }

  std::shared_ptr<Resource> load(const std::string &path) {
    SDL_RWops *sdlFile = SDL_RWFromFile(path.c_str(), "rb");

    int size = SDL_RWsize(sdlFile);
    std::vector<char> buffer(size);
    SDL_RWread(sdlFile, buffer.data(), size, 1);
    SDL_RWclose(sdlFile);

    std::string resourceContents(buffer.begin(), buffer.end());

    rapidjson::Document document;
    document.Parse(resourceContents.c_str());

    spdlog::debug("Vert: {0} Frag: {1}", document["vertex_code"].GetString(),
                  document["fragment_code"].GetString());

    std::vector<char> vertCode = readSpirv(document["vertex_code"].GetString());
    std::vector<char> fragCode =
        readSpirv(document["fragment_code"].GetString());

    std::shared_ptr<VulkanPipelineResource> ptr(new VulkanPipelineResource(
        device, params, renderPass, vertCode, fragCode));
    ptr->setResourceType(RESOURCE_VULKAN_PIPELINE);
    return std::static_pointer_cast<Resource>(ptr);
  }
};