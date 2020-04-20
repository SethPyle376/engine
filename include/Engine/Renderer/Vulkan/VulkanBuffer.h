#pragma once

#include "volk.h"
#include "vk_mem_alloc.h"

class VulkanBuffer {
private:
    VkBuffer buffer;
public:
    VulkanBuffer();
};