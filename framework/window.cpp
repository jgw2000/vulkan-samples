#include "window.h"

namespace vks
{
    Window::Window(std::string name, uint32_t w, uint32_t h)
        : title(name), width(w), height(h)
    {
        __Init();
    }

    Window::~Window()
    {
        glfwDestroyWindow(handle);
        glfwTerminate();
    }

    std::vector<const char*> Window::GetRequiredExtensions() const
    {
        uint32_t count{ 0 };
        const char** names = glfwGetRequiredInstanceExtensions(&count);
        return { names, names + count };
    }

    VkSurfaceKHR Window::CreateSurface(VkInstance instance)
    {
        if (instance == VK_NULL_HANDLE || !handle)
            return VK_NULL_HANDLE;

        VkSurfaceKHR surface;
        VkResult errCode = glfwCreateWindowSurface(instance, handle, NULL, &surface);
        if (errCode != VK_SUCCESS)
            return nullptr;

        return surface;
    }

    void Window::__Init()
    {
        if (glfwInit() != GLFW_TRUE)
            throw std::runtime_error("GLFW init failed!");

        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

        handle = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
    }
}