#include "Engine/Resources/ResourceManager.h"

ResourceManager *ResourceManager::instance = 0;

ResourceManager::ResourceManager() {
  resourceMap = std::unordered_map<std::string, std::weak_ptr<Resource>>();
}

void ResourceManager::registerFactory(ResourceFactory *factory) {
  factoryMap[factory->resourceType] = std::unique_ptr<ResourceFactory>(factory);
}

std::shared_ptr<Resource> ResourceManager::loadResource(std::string filepath) {
  SDL_RWops *sdlFile = SDL_RWFromFile(filepath.c_str(), "rb");

  if (sdlFile == nullptr) {
    spdlog::error(SDL_GetError());
  }

  int size = SDL_RWsize(sdlFile);
  std::vector<char> buffer(size);
  SDL_RWread(sdlFile, buffer.data(), size, 1);

  SDL_RWclose(sdlFile);

  std::string resourceData(buffer.begin(), buffer.end());

  rapidjson::Document document;
  document.Parse(resourceData.c_str());

  std::string resourceType = document["type"].GetString();
  std::shared_ptr<Resource> resource =
      (factoryMap[resourceType]->load(filepath));
  resourceMap[filepath] = std::weak_ptr<Resource>(resource);
  return resource;
}

std::shared_ptr<Resource> ResourceManager::getResource(std::string filepath) {
  std::unordered_map<std::string, std::weak_ptr<Resource>>::iterator it =
      resourceMap.find(filepath);

  if (it != resourceMap.end()) {
    if (!resourceMap[filepath].expired()) {
      return resourceMap[filepath].lock();
    }
  }
  return loadResource(filepath);
}

std::vector<std::shared_ptr<Resource>>
ResourceManager::getResources(RESOURCE_TYPE type) {
  std::unordered_map<std::string, std::weak_ptr<Resource>>::iterator it =
      resourceMap.begin();
  std::vector<std::shared_ptr<Resource>> resources;
  for (it; it != resourceMap.end(); it++) {
    if (!it->second.expired()) {
      std::shared_ptr<Resource> resource = it->second.lock();
      if (resource->getResourceType() == type) {
        resources.push_back(resource);
      }
    }
  }
  return resources;
}

ResourceManager *ResourceManager::getInstance() {
  if (instance == 0) {
    instance = new ResourceManager();
  }
  return instance;
}