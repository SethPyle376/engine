#pragma once

#include <vector>
#include <stdexcept>

#include "volk.h"

#include "SDL.h"
#undef main
#include "SDL_vulkan.h"

#include "Engine/Renderer/Renderer.h"

#include "Engine/Renderer/Vulkan/VulkanUtils.h"
#include "Engine/Renderer/Vulkan/VulkanSwapchain.h"
#include "Engine/Renderer/Vulkan/VulkanFramebuffer.h"
#include "Engine/Renderer/Vulkan/VulkanBuffer.h"
#include "Engine/Renderer/Vulkan/VulkanVertex.h"

#include "Engine/Resources/ResourceManager.h"
#include "Engine/Renderer/Vulkan/Resources/VulkanPipelineResource.h"
#include "Engine/Renderer/Vulkan/Resources/VulkanMeshResource.h"

const std::vector<const char*> validationLayers = {
    "VK_LAYER_LUNARG_standard_validation"
};

const std::vector<const char*> deviceExtensions = {
    VK_KHR_SWAPCHAIN_EXTENSION_NAME
};

const int MAX_FRAMES_IN_FLIGHT = 2;

class VulkanRenderer {
private:
    ResourceManager* resourceManager;
    #ifdef NDEBUG
        const bool enableValidationLayers = false;
        std::vector<const char*> extensions;
    #else
        const bool enableValidationLayers = true;
        std::vector<const char*> extensions = {
            VK_EXT_DEBUG_REPORT_EXTENSION_NAME
        };
    #endif
    RendererParams params;
    SDL_Window* window;

    VkInstance instance;

    VulkanDevice* device;
    VulkanSwapchain* swapchain;

    VkViewport viewport;

    std::vector<VulkanFramebuffer> framebuffers;
    
    std::vector<VkCommandBuffer> commandBuffers;

    VkPhysicalDeviceFeatures deviceFeatures = {};

    VkRenderPass renderPass;
    VkRect2D scissor;

    std::vector<VkSemaphore> imageAvailableSemaphores;
    std::vector<VkSemaphore> renderFinishedSemaphores;
    std::vector<VkFence> inFlightFences;
    std::vector<VkFence> imagesInFlight;

    size_t currentFrame = 0;

    void initSDL();
    void initWindow();
    void initViewport();
    void initVolk();
    void createInstance();
    void initSwapchain();
    VkPhysicalDevice pickPhysicalDevice();
    void initLogicalDevice();
    void initRenderPass();
    void initFramebuffers();
    void initCommandBuffers();
    void initSemaphores();
public:
    VulkanRenderer(const RendererParams &params);
    ~VulkanRenderer();
    bool checkValidationLayerSupport();
    void init();
    void beginFrame();
    void drawFrame();
    void finishFrame();
    void buildCommandbuffers();
    void recreateSwapchain();

    VulkanDevice* getDevice();
    VkRenderPass getRenderPass();
};