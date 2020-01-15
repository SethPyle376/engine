#pragma once

#include <string>
#include <memory>

#include "Engine/Resources/Resource.h"

class ResourceFactory {
public:
    std::string resourceType = "none";
    virtual Resource load(const std::string &path) = 0;
};