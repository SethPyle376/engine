#include "Engine/Resources/ResourceManager.h"

ResourceManager::ResourceManager() {
    resourceMap = std::unordered_map<std::string, std::weak_ptr<Resource>>();
}

std::shared_ptr<Resource> ResourceManager::loadResource(std::string filepath) {
    SDL_RWops *sdlFile = SDL_RWFromFile(filepath.c_str(), "rb");

    int size = SDL_RWsize(sdlFile);
    std::vector<char> buffer(size);
    SDL_RWread(sdlFile, buffer.data(), size, 1);

    SDL_RWclose(sdlFile);

    rapidjson::Document document;
    document.Parse(buffer.data());

    std::string resourceType =  document["type"].GetString();
    std::shared_ptr<Resource> resource = (factoryMap[resourceType]->load(filepath));
    resourceMap[filepath] = std::weak_ptr<Resource>(resource);
    return resource;
}

std::shared_ptr<Resource> ResourceManager::getResource(std::string filepath) {
    std::unordered_map<std::string, std::weak_ptr<Resource>>::iterator it = resourceMap.find(filepath);

    if (it != resourceMap.end()) {
        return resourceMap[filepath].lock();
    } else {
        return loadResource(filepath);
    }
}