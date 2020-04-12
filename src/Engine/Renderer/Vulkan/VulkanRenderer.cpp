#include "Engine/Renderer/Vulkan/VulkanRenderer.h"

VulkanRenderer::VulkanRenderer(const RendererParams &params) {
    this->params = params;
}

VulkanRenderer::~VulkanRenderer() {
    spdlog::debug("destroying vulkan renderer");

    vkDestroyRenderPass(device->getDevice(), renderPass, nullptr);

    delete swapchain;
    delete device;
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
    swapchain = new VulkanSwapchain(instance);
    swapchain->initSurface(window);
    volkLoadInstance(instance);
    device = new VulkanDevice(pickPhysicalDevice());
    device->createLogicalDevice(deviceFeatures, deviceExtensions, nullptr);
    swapchain->connect(device->getPhysicalDevice(), device->getDevice());
    swapchain->create(params.x, params.y);
    initRenderPass();
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

void VulkanRenderer::initSwapchain() {
    swapchain->initSurface(window);
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
        if (isDeviceSuitable(device, swapchain->getSurface(), deviceExtensions)) {
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

VkDevice VulkanRenderer::getLogicalDevice() {
    return device->getDevice();
}

VkRenderPass VulkanRenderer::getRenderPass() {
    return renderPass;
}

void VulkanRenderer::initRenderPass() {
    VkAttachmentDescription colorAttachment = {};
    colorAttachment.format = swapchain->getImageFormat();
    colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
    colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

    VkAttachmentReference colorAttachmentRef = {};
    colorAttachmentRef.attachment = 0;
    colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

    VkSubpassDescription subpass = {};
    subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    subpass.colorAttachmentCount = 1;
    subpass.pColorAttachments = &colorAttachmentRef;

    VkRenderPassCreateInfo renderPassInfo = {};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    renderPassInfo.attachmentCount = 1;
    renderPassInfo.pAttachments = &colorAttachment;
    renderPassInfo.subpassCount = 1;
    renderPassInfo.pSubpasses = &subpass;

    if (vkCreateRenderPass(device->getDevice(), &renderPassInfo, nullptr, &renderPass) != VK_SUCCESS) {
        spdlog::error("failed to create render pass");
    } else {
        spdlog::debug("created render pass");
    }
}