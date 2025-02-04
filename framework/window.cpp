#include "window.h"

#include <GLFW/glfw3.h>
#include <stdexcept>

namespace vkb
{
    Window::Window(const Properties& properties) :
        properties{ properties }
    {
        if (!glfwInit())
        {
            throw std::runtime_error("GLFW couldn't be initialized.");
        }

        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

        switch (properties.mode)
        {
            case Window::Mode::Fullscreen:
            {
                auto* monitor = glfwGetPrimaryMonitor();
                const auto* mode = glfwGetVideoMode(monitor);
                handle = glfwCreateWindow(mode->width, mode->height, properties.title.c_str(), monitor, NULL);
                break;
            }

            case Window::Mode::FullscreenBorderless:
            {
                auto* monitor = glfwGetPrimaryMonitor();
                const auto* mode = glfwGetVideoMode(monitor);
                glfwWindowHint(GLFW_RED_BITS, mode->redBits);
                glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
                glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
                glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);
                handle = glfwCreateWindow(mode->width, mode->height, properties.title.c_str(), monitor, NULL);
                break;
            }

            default:
                handle = glfwCreateWindow(properties.extent.width, properties.extent.height, properties.title.c_str(), NULL, NULL);
                break;
        }

        resize(Extent{ properties.extent.width, properties.extent.height });

        if (!handle)
        {
            throw std::runtime_error("Couldn't create glfw window.");
        }
    }

    Window::~Window()
    {
        glfwTerminate();
    }

    VkSurfaceKHR Window::create_surface(VkInstance instance)
    {
        if (instance == VK_NULL_HANDLE || !handle)
        {
            return VK_NULL_HANDLE;
        }

        VkSurfaceKHR surface;
        VkResult errCode = glfwCreateWindowSurface(instance, handle, NULL, &surface);

        if (errCode != VK_SUCCESS)
        {
            return nullptr;
        }

        return surface;
    }

    bool Window::should_close()
    {
        return glfwWindowShouldClose(handle);
    }

    void Window::close()
    {
        glfwSetWindowShouldClose(handle, GLFW_TRUE);
    }

    void Window::process_events()
    {
        glfwPollEvents();
    }

    Window::Extent Window::resize(const Extent& new_extent)
    {
        if (properties.resizable)
        {
            properties.extent.width = new_extent.width;
            properties.extent.height = new_extent.height;
        }

        return properties.extent;
    }

    const Window::Extent& Window::get_extent() const
    {
        return properties.extent;
    }

    Window::Mode Window::get_window_mode() const
    {
        return properties.mode;
    }

    float Window::get_content_scale_factor() const
    {
        return 1.0f;
    }

    std::vector<const char*> Window::get_required_surface_extensions() const
    {
        uint32_t glfw_extension_count = 0;
        const char** names = glfwGetRequiredInstanceExtensions(&glfw_extension_count);
        return { names, names + glfw_extension_count };
    }
}