#include "Engine/Renderer/Vulkan/VulkanRenderer.h"

VulkanRenderer::VulkanRenderer(const RendererParams &params) {
    this->params = params;
}

VulkanRenderer::~VulkanRenderer() {
    spdlog::debug("destroying vulkan renderer");

    delete device;
    vkDestroySurfaceKHR(instance, surface, nullptr);
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
            spdlog::debug("layer: " + std::string(layerProperties.layerName));
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
    spdlog::set_level(spdlog::level::debug);
    initSDL();
    initVolk();
    initWindow();
    createInstance();
    volkLoadInstance(instance);
    initSurface();
    device = new VulkanDevice(pickPhysicalDevice());
    device->createLogicalDevice(deviceFeatures, deviceExtensions, nullptr);
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
        spdlog::debug("initializing window");
        Uint32 windowFlags = SDL_WINDOW_RESIZABLE | SDL_WINDOW_VULKAN;
        window = SDL_CreateWindow("Titus v0.1", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, params.x, params.y, windowFlags);
}

void VulkanRenderer::createInstance() {
    spdlog::debug("initializing vulkan instance");
    bool validationLayerSupport = checkValidationLayerSupport();
    if (enableValidationLayers && !validationLayerSupport) {
        spdlog::error("validation layers not supported");
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
        spdlog::error("failed to get required vulkan extension count from sdl2");
    }

    size_t additional_count = extensions.size();
    extensions.resize(additional_count + extensionCount);

    if (!SDL_Vulkan_GetInstanceExtensions(window, &extensionCount, extensions.data() + additional_count)) {
        spdlog::error("failed to get required vulkan extensions from sdl2");
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
        spdlog::error("failed to initialize instance");
    } else {
        spdlog::debug("instance initialized");
    }
}

void VulkanRenderer::initSurface() {
    if (SDL_Vulkan_CreateSurface(window, instance, &surface) != SDL_TRUE) {
        spdlog::error("failed to initialize vulkan surface");
    } else {
        spdlog::debug("initialized vulkan surface");
    }
}

VkPhysicalDevice VulkanRenderer::pickPhysicalDevice() {
    spdlog::debug("selecting vulkan physical device");

    VkPhysicalDevice physicalDevice;

    uint32_t deviceCount = 0;

    vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);

    if (deviceCount == 0) {
        spdlog::error("no physical devices with vulkan support found");
    }

    std::vector<VkPhysicalDevice> devices(deviceCount);
    vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());

    for (const auto& device : devices) {
        if (isDeviceSuitable(device, surface, deviceExtensions)) {
            physicalDevice = device;
        }
        break;
    }

    if (physicalDevice == VK_NULL_HANDLE) {
        spdlog::error("failed to pick Vulkan physical device");
        assert(physicalDevice != VK_NULL_HANDLE);
    } else {
        spdlog::debug("selected Vulkan physical device");
        logDeviceProperties(physicalDevice);
    }
    return physicalDevice;
}