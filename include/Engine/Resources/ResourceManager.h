#pragma once

#include <string>
#include <map>
#include <pointer.h>

#include "Engine/Resources/Resource.h"

class ResourceManager {
private:
    std::map<std::string, std::weak_ptr<Resource>> resourceMap;

    std::shared_ptr<Resource> loadResource(std::string filename);

public:
    ResourceManager();

    Resource* getResource(std::string filepath);
};