#pragma once

#include "Engine/Resources/MockResource.h"

#include "Engine/Resources/ResourceFactory.h"

class MockResourceFactory : public ResourceFactory {
private:
public:
  MockResourceFactory() { this->resourceType = "mock"; }

  virtual std::shared_ptr<Resource> load(const std::string &path) {
    std::string pathCopy = path;
    std::shared_ptr<MockResource> ptr(new MockResource());
    return std::static_pointer_cast<Resource>(ptr);
  }
};