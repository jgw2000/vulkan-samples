#pragma once

#include "window.h"

namespace vks
{
    class BaseApp
    {
    public:
        BaseApp();
        BaseApp(std::string title, uint32_t width, uint32_t height);
        
        ~BaseApp() = default;

        BaseApp(const BaseApp&) = delete;
        BaseApp& operator=(const BaseApp&) = delete;

        BaseApp(const BaseApp&&) = delete;
        BaseApp& operator=(const BaseApp&&) = delete;

        void Run();

    protected:
        virtual void Prepare();
        virtual void Update();
        virtual void Finish();

    private:
        std::unique_ptr<Window> window;

        vk::Instance instance;
    };
}