#include "Engine/Resources/ResourceManager.h"

ResourceManager::ResourceManager() {
    resourceMap = std::map<std::string, Resource*>();
}

Resource* ResourceManager::getResource(std::string filepath) {
    std::map<std::string, Resource*>::iterator it = resourceMap.find(filepath);

}