#include "hpp_hello_triangle.h"

VULKAN_HPP_DEFAULT_DISPATCH_LOADER_DYNAMIC_STORAGE

namespace vks
{
    void HppHelloTriangle::Prepare()
    {
        __CreateInstance();
    }

    void HppHelloTriangle::Finish()
    {
        __instance.destroy();
    }

    void HppHelloTriangle::__CreateInstance()
    {
        static vk::DynamicLoader dl;
        PFN_vkGetInstanceProcAddr vkGetInstanceProcAddr = dl.getProcAddress<PFN_vkGetInstanceProcAddr>("vkGetInstanceProcAddr");
        VULKAN_HPP_DEFAULT_DISPATCHER.init(vkGetInstanceProcAddr);

        vk::ApplicationInfo app("Hello Triangle", {}, "Vulkan", {}, VK_MAKE_VERSION(1, 0, 0));
        vk::InstanceCreateInfo instanceInfo({}, &app, {}, {});

        __instance = vk::createInstance(instanceInfo);

        // initialize function pointers for instance
        VULKAN_HPP_DEFAULT_DISPATCHER.init(__instance);
    }
}