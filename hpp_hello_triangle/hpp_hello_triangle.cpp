#include "hpp_hello_triangle.h"

namespace vks
{
    void HppHelloTriangle::Prepare()
    {
        __instance = CreateInstance({ VK_KHR_SURFACE_EXTENSION_NAME }, {});
        __SelectPhysicalDevice();
        __device = CreateDevice(__physical_device, { VK_KHR_SWAPCHAIN_EXTENSION_NAME }, __graphics_queue_index.value());
        __graphics_queue = __device.getQueue(__graphics_queue_index.value(), 0);
        __surface = static_cast<vk::SurfaceKHR>(window->CreateSurface(static_cast<VkInstance>(__instance)));
    }

    void HppHelloTriangle::Update()
    {

    }

    void HppHelloTriangle::Finish()
    {
        __instance.destroySurfaceKHR(__surface);
        __device.destroy();
        __instance.destroy();
    }

    void HppHelloTriangle::__SelectPhysicalDevice()
    {
        std::vector<vk::PhysicalDevice> physicalDevices = __instance.enumeratePhysicalDevices();
        for (const auto& physicalDevice : physicalDevices) {
            auto physicalDeviceProperties = physicalDevice.getProperties();
            auto physicalDeviceFeatures = physicalDevice.getFeatures();
            auto queueFamilyProperties = physicalDevice.getQueueFamilyProperties();

            if (physicalDeviceProperties.deviceType != vk::PhysicalDeviceType::eDiscreteGpu)
                continue;

            bool foundGraphicsQueue = false;
            for (uint32_t i = 0; i < queueFamilyProperties.size(); ++i) {
                if (queueFamilyProperties[i].queueFlags & vk::QueueFlagBits::eGraphics) {
                    foundGraphicsQueue = true;
                    __graphics_queue_index = i;
                    __physical_device = physicalDevice;
                    break;
                }
            }

            if (!foundGraphicsQueue)
                continue;

            return;
        }

        throw std::runtime_error("Failed to find a suitable gpu.");
    }
}