#include "Engine/Renderer/Vulkan/VulkanRenderer.h"

VulkanRenderer::VulkanRenderer(const RendererParams &params) {
    this->params = params;
}

VulkanRenderer::~VulkanRenderer() {
    vkDestroyInstance(instance, nullptr);
    SDL_DestroyWindow(window);
}

bool VulkanRenderer::checkValidationLayerSupport() {
    uint32_t layerCount;
    vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

    std::vector<VkLayerProperties> availableLayers(layerCount);
    vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());
    
    for (const char* layerName : validationLayers) {
        bool layerFound = false;

        for (const auto& layerProperties : availableLayers) {
            if (strcmp(layerName, layerProperties.layerName) == 0) {
                layerFound = true;
                break;
            }
        }

        if (!layerFound) {
            return false;
        }
    }
    
    return true;
}

void VulkanRenderer::init() {
    initSDL();
    initVolk();
    initWindow();
    createInstance();
}

void VulkanRenderer::beginFrame() {
    return;
}

void VulkanRenderer::endFrame() {
    return;
}

void VulkanRenderer::initSDL() {
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS);
    SDL_Vulkan_LoadLibrary(NULL);
}

void VulkanRenderer::initVolk() {
    volkInitializeCustom((PFN_vkGetInstanceProcAddr)SDL_Vulkan_GetVkGetInstanceProcAddr());
}

void VulkanRenderer::initWindow() {
        Uint32 windowFlags = SDL_WINDOW_RESIZABLE | SDL_WINDOW_VULKAN;
        window = SDL_CreateWindow("Titus v0.1", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, params.x, params.y, windowFlags);
}

void VulkanRenderer::createInstance() {
    if (enableValidationLayers && !checkValidationLayerSupport()) {
        throw std::runtime_error("validation layers unsupported");
    }

    VkApplicationInfo appInfo = {};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = "Titus";
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pEngineName = "Titus";
    appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.apiVersion = VK_API_VERSION_1_0;

    unsigned int extensionCount;
    if (!SDL_Vulkan_GetInstanceExtensions(window, &extensionCount, NULL)) {
        //log error
    }

    size_t additional_count = extensions.size();
    extensions.resize(additional_count + extensionCount);

    if (!SDL_Vulkan_GetInstanceExtensions(window, &extensionCount, extensions.data() + additional_count)) {
        //TODO: ERROR HANDLER
    };

    VkInstanceCreateInfo createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &appInfo;
    createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
    createInfo.ppEnabledExtensionNames = extensions.data();

    if (enableValidationLayers) {
        createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
        createInfo.ppEnabledLayerNames = validationLayers.data();
    } else {
        createInfo.enabledLayerCount = 0;
    }

    VkResult result = vkCreateInstance(&createInfo, nullptr, &instance);

    if (result != VK_SUCCESS) {

    }
}