#pragma once

#include <string>

#include "Engine/Resources/Utils/Resource_Types.h"

class Resource {
private:
    int resource_id;
    std::string filename;
    RESOURCE_TYPE type;

    bool loaded;

public:
    Resource(std::string filename) : filename(filename), resource_id(0), type(RESOURCE_NULL) {}

    void setResourceId(int resource_id) { this->resource_id = resource_id; }
    int getResourceId() { return resource_id; }

    void setFilename(std::string filename) { this->filename = filename; }
    std::string getFilename() { return filename; }

    void setResourceType(RESOURCE_TYPE type) { this->type = type; }
    RESOURCE_TYPE getResourceType() { return type; }

    virtual void load() = 0;
};