#pragma once

#include <string>

#include "Engine/Resources/Resource.h"

class MockResource : public Resource {
public:
    MockResource(std::string path) : Resource(path) {
        payload = "Hello from mock resource";
    }
    std::string payload;
};