#pragma once

#include <string>

#include "Engine/Resources/Utils/Resource_Types.h"

class Resource {
private:
    int resource_id;
    RESOURCE_TYPE type;

    bool loaded;

public:
    Resource() : resource_id(0), type(RESOURCE_NULL) {}

    void setResourceId(int resource_id) { this->resource_id = resource_id; }
    int getResourceId() { return resource_id; }

    void setResourceType(RESOURCE_TYPE type) { this->type = type; }
    RESOURCE_TYPE getResourceType() { return type; }
};