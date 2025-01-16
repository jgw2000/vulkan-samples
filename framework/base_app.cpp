#include "base_app.h"

VULKAN_HPP_DEFAULT_DISPATCH_LOADER_DYNAMIC_STORAGE

namespace vks
{
    BaseApp::BaseApp()
    {
        window = std::make_unique<Window>();
    }

    BaseApp::BaseApp(std::string title, uint32_t width, uint32_t height)
    {
        window = std::make_unique<Window>(title, width, height);
    }

    void BaseApp::Run()
    {
        Prepare();

        while (!window->ShouldClose()) {
            window->ProcessEvents();
            Update();
        }

        Finish();
    }

    void BaseApp::Prepare()
    {
        
    }

    void BaseApp::Update()
    {

    }

    void BaseApp::Finish()
    {

    }

    vk::Instance BaseApp::CreateInstance(const std::vector<const char*>& required_instance_extensions, const std::vector<const char*>& required_validation_layers)
    {
        static vk::DynamicLoader dl;
        PFN_vkGetInstanceProcAddr vkGetInstanceProcAddr = dl.getProcAddress<PFN_vkGetInstanceProcAddr>("vkGetInstanceProcAddr");
        VULKAN_HPP_DEFAULT_DISPATCHER.init(vkGetInstanceProcAddr);

        // Check instance extensions
        std::vector<vk::ExtensionProperties> available_instance_extensions = vk::enumerateInstanceExtensionProperties();
        std::vector<const char*> active_instance_extensions(required_instance_extensions);
#ifdef _DEBUG
        active_instance_extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
#endif
        if (!__ValidateExtensions(active_instance_extensions, available_instance_extensions))
            throw std::runtime_error("Required instance extensions are missing.");

        // Check validation layers
        std::vector<vk::LayerProperties> supported_validation_layers = vk::enumerateInstanceLayerProperties();
        std::vector<const char*> requested_validation_layers(required_validation_layers);
#ifdef _DEBUG
        requested_validation_layers.push_back("VK_LAYER_KHRONOS_validation");
#endif
        if (!__ValidateLayers(requested_validation_layers, supported_validation_layers))
            throw std::runtime_error("Required validation layers are missing.");

        vk::ApplicationInfo app("Vulkan Sample", {}, "Vulkan", {}, VK_MAKE_VERSION(1, 0, 0));
        vk::InstanceCreateInfo instanceInfo({}, &app, requested_validation_layers, active_instance_extensions);

        vk::Instance instance = vk::createInstance(instanceInfo);

        // initialize function pointers for instance
        VULKAN_HPP_DEFAULT_DISPATCHER.init(instance);

        return instance;
    }

    vk::Device BaseApp::CreateDevice(const vk::PhysicalDevice& physicalDevice, const std::vector<const char*>& required_device_extensions, uint32_t graphics_queue_index)
    {
        std::vector<vk::ExtensionProperties> device_extensions = physicalDevice.enumerateDeviceExtensionProperties();

        if (!__ValidateExtensions(required_device_extensions, device_extensions))
            throw std::runtime_error("Required device extensions are missing.");

        // Create a device with one graphics queue
        float queue_priority = 1.0f;
        vk::DeviceQueueCreateInfo queue_info({}, graphics_queue_index, 1, &queue_priority);
        vk::DeviceCreateInfo device_info({}, queue_info, {}, required_device_extensions);
        vk::Device device = physicalDevice.createDevice(device_info);

        // initialize function pointers for device
        VULKAN_HPP_DEFAULT_DISPATCHER.init(device);

        return device;
    }

    bool BaseApp::__ValidateExtensions(const std::vector<const char*>& required, const std::vector<vk::ExtensionProperties>& available)
    {
        return std::find_if(required.begin(), required.end(),
            [&available](auto extension) {
                return std::find_if(available.begin(), available.end(),
                    [&extension](const auto& ep) {
                        return strcmp(ep.extensionName, extension) == 0;
                    }) == available.end();
            }) == required.end();
    }

    bool BaseApp::__ValidateLayers(const std::vector<const char*>& required, const std::vector<vk::LayerProperties>& available)
    {
        return std::find_if(required.begin(), required.end(),
            [&available](auto layer) {
                return std::find_if(available.begin(), available.end(),
                    [&layer](const auto& lp) {
                        return strcmp(lp.layerName, layer) == 0;
                    }) == available.end();
            }) == required.end();
    }
}