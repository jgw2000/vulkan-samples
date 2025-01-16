#pragma once

#include "window.h"

#define VULKAN_HPP_DISPATCH_LOADER_DYNAMIC 1

namespace vks
{
    class BaseApp
    {
    public:
        BaseApp();
        BaseApp(std::string title, uint32_t width, uint32_t height);
        
        ~BaseApp() = default;

        BaseApp(const BaseApp&) = delete;
        BaseApp& operator=(const BaseApp&) = delete;

        BaseApp(const BaseApp&&) = delete;
        BaseApp& operator=(const BaseApp&&) = delete;

        void Run();

    protected:
        struct SwapchainData
        {
            vk::Extent2D                    extent;
            vk::Format                      format = vk::Format::eUndefined;
            vk::SwapchainKHR                swapchain;
            std::vector<vk::ImageView>      imageViews;
            std::vector<vk::Framebuffer>    framebuffers;
        };

        virtual void Prepare() = 0;
        virtual void Update() = 0;
        virtual void Finish() = 0;

        vk::Instance    CreateInstance(const std::vector<const char*>& required_instance_extensions, const std::vector<const char*>& required_validation_layers);
        vk::Device      CreateDevice(const vk::PhysicalDevice& physicalDevice, const std::vector<const char*>& required_device_extensions, uint32_t graphics_queue_index);

        std::unique_ptr<Window> window;

    private:
        bool __ValidateExtensions(const std::vector<const char*>& required, const std::vector<vk::ExtensionProperties>& available);
        bool __ValidateLayers(const std::vector<const char*>& required, const std::vector<vk::LayerProperties>& available);
    };
}