#pragma once

#include "base_app.h"

#include <optional>

namespace vks
{
    class HppHelloTriangle : public BaseApp
    {
    public:
        HppHelloTriangle() = default;
        HppHelloTriangle(std::string title, uint32_t width, uint32_t height) : BaseApp(title, width, height) {}

        HppHelloTriangle(const HppHelloTriangle&) = delete;
        HppHelloTriangle& operator=(const HppHelloTriangle&) = delete;

        HppHelloTriangle(const HppHelloTriangle&&) = delete;
        HppHelloTriangle& operator=(const HppHelloTriangle&&) = delete;

    protected:
        virtual void Prepare() override;
        virtual void Update() override;
        virtual void Finish() override;

    private:
        void __SelectPhysicalDevice();

        // Setup
        vk::Instance            __instance;
        vk::PhysicalDevice      __physical_device;
        std::optional<uint32_t> __graphics_queue_index{};
        vk::Device              __device;
        vk::Queue               __graphics_queue;

        // Presentation
        vk::SurfaceKHR          __surface;
        SwapchainData           __swapchain_data;
    };
}