#pragma once

#include <string>

#include "Engine/Resources/ResourceFactory.h"
#include "Engine/Renderer/Vulkan/Resources/VulkanPipelineResource.h"

class VulkanPipelineResourceFactory : public ResourceFactory {
private:
    VkDevice device;

    std::vector<char> readSpirv(const std::string &path) {
        SDL_RWops *sdlFile = SDL_RWFromFile(path.c_str(), "rb");

        int size = SDL_RWsize(sdlFile);
        std::vector<char> buffer(size);

        SDL_RWread(sdlFile, buffer.data(), size, 1);
        SDL_RWclose(sdlFile);

        return buffer;
    }

public:
    VulkanPipelineResourceFactory(VkDevice device) {
        this->device = device;
        this->resourceType = "vulkan_shader";
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

        spdlog::debug("Vert: {0} Frag: {1}", document["vertex_code"].GetString(), document["fragment_code"].GetString());
        

        std::vector<char> vertCode = readSpirv(document["vertex_code"].GetString());
        std::vector<char> fragCode = readSpirv(document["fragment_code"].GetString());

        std::shared_ptr<VulkanPipelineResource> ptr(new VulkanPipelineResource(device, vertCode, fragCode));
        return std::static_pointer_cast<Resource>(ptr);
    }
};