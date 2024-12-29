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

    void Window::__Init()
    {
        if (glfwInit() != GLFW_TRUE)
            throw std::runtime_error("GLFW init failed!");

        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

        handle = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
    }
}