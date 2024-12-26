#include "base_app.h"

namespace vks
{
    BaseApp::BaseApp(std::string title, uint32_t width, uint32_t height)
    {
        window = std::make_unique<Window>(title, width, height);
    }

    void BaseApp::Run()
    {
        while (!window->ShouldClose()) {
            window->ProcessEvents();
        }
    }
}