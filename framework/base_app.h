#pragma once

#include "window.h"

#include <memory>

namespace vks
{
    class BaseApp
    {
    public:
        BaseApp(std::string title = "Vulkan", uint32_t width = 800, uint32_t height = 600);
        ~BaseApp() = default;

        BaseApp(const BaseApp&) = delete;
        BaseApp& operator=(const BaseApp&) = delete;

        BaseApp(const BaseApp&&) = delete;
        BaseApp& operator=(const BaseApp&&) = delete;

        void Run();

    private:
        std::unique_ptr<Window> window;
    };
}