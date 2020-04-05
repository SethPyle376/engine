#pragma once

#include <string>

#include "Engine/Resources/ResourceFactory.h"
#include "Engine/Renderer/Vulkan/Resources/VulkanShaderResource.h"

class VulkanShaderResourceFactory : public ResourceFactory {
private:

public:
    VulkanShaderResourceFactory() {
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
        std::shared_ptr<VulkanShaderResource> ptr(new VulkanShaderResource(document["vertex_code"].GetString(), document["fragment_code"].GetString()));
        return std::static_pointer_cast<Resource>(ptr);
    }
};