#include "Engine/Scene/Actor.h"

Actor::Actor(std::shared_ptr<VulkanMeshInstanceResource> meshInstance)
{
  this->nodeType = ACTOR;
  this->meshInstance = meshInstance;
}

Actor::~Actor() {
  
}

std::shared_ptr<VulkanMeshInstanceResource> Actor::getMeshInstance()
{
  return meshInstance;
}

void Actor::update() {
  static auto startTime = std::chrono::high_resolution_clock::now();
  auto currentTime = std::chrono::high_resolution_clock::now();
  float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();


  rotation = glm::rotate(glm::mat4(1.0f), time * glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
}