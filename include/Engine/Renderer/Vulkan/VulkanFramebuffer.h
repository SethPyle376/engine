#pragma once

#include "volk.h"

struct VulkanFramebuffer {
private:
    VkDevice device;
public:
    VulkanFramebuffer(VkDevice device) {
        this->device = device;
    }

    ~VulkanFramebuffer() {
        vkDestroyFramebuffer(device, framebuffer, nullptr);
        spdlog::debug("destroying framebuffer");
    }

    VkFramebuffer framebuffer;
};