#pragma once

#include "base_app.h"

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
        virtual void Finish() override;

    private:
        void __CreateInstance();

        vk::Instance __instance;
    };
}