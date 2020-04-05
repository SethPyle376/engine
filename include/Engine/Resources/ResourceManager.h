#pragma once

#include <string>
#include <unordered_map>
#include <memory>
#include <vector>

#include "SDL.h"
#include "document.h"

#include "Engine/Resources/Resource.h"
#include "Engine/Resources/ResourceFactory.h"

class ResourceManager {
private:
    std::unordered_map<std::string, std::weak_ptr<Resource>> resourceMap;
    std::unordered_map<std::string, std::unique_ptr<ResourceFactory>> factoryMap;

    std::shared_ptr<Resource> loadResource(std::string filename);

public:
    ResourceManager();

    void registerFactory(ResourceFactory* factory);

    std::shared_ptr<Resource> getResource(std::string filepath);
};