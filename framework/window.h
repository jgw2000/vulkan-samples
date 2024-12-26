#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <string>

namespace vks
{
    class Window
    {
    public:
        Window(std::string name, uint32_t w, uint32_t h);
        ~Window();

        Window(const Window&) = delete;
        Window& operator=(const Window&) = delete;

        Window(const Window&&) = delete;
        Window& operator=(const Window&&) = delete;

        bool ShouldClose() const { return glfwWindowShouldClose(handle); }
        void ProcessEvents() const { glfwPollEvents(); }

    private:
        void __Init();

    private:
        GLFWwindow* handle;

        std::string title;
        uint32_t width;
        uint32_t height;
    };
}