#pragma once

#include <memory>
#include <string>

#include "Engine/Resources/Resource.h"

class ResourceFactory {
public:
  std::string resourceType = "none";
  virtual std::shared_ptr<Resource> load(const std::string &path) = 0;
};