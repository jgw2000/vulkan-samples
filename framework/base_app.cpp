#include "base_app.h"

namespace vks
{
    BaseApp::BaseApp()
    {
        window = std::make_unique<Window>();
    }

    BaseApp::BaseApp(std::string title, uint32_t width, uint32_t height)
    {
        window = std::make_unique<Window>(title, width, height);
    }

    void BaseApp::Run()
    {
        Prepare();

        while (!window->ShouldClose()) {
            window->ProcessEvents();
            Update();
        }

        Finish();
    }

    void BaseApp::Prepare()
    {
        
    }

    void BaseApp::Update()
    {

    }

    void BaseApp::Finish()
    {

    }
}