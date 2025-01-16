#pragma once

#include "api.h"

#include <GLFW/glfw3.h>

namespace vks
{
    class Window
    {
    public:
        Window(std::string name = "Vulkan", uint32_t w = 800, uint32_t h = 600);
        ~Window();

        Window(const Window&) = delete;
        Window& operator=(const Window&) = delete;

        Window(const Window&&) = delete;
        Window& operator=(const Window&&) = delete;

        bool ShouldClose() const { return glfwWindowShouldClose(handle); }
        void ProcessEvents() const { glfwPollEvents(); }

        std::vector<const char*> GetRequiredExtensions() const;

        VkSurfaceKHR CreateSurface(VkInstance instance);

    private:
        void __Init();

    private:
        GLFWwindow* handle;

        std::string title;
        uint32_t width;
        uint32_t height;
    };
}