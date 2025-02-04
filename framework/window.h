#pragma once

#define VK_NO_PROTOTYPES
#include <vulkan/vulkan.hpp>
#include <string>
#include <vector>

struct GLFWwindow;

namespace vkb
{
    class Window
    {
    public:
        struct Extent
        {
            uint32_t width;
            uint32_t height;
        };

        enum class Mode
        {
            Headless,
            Fullscreen,
            FullscreenBorderless,
            Default
        };

        enum class Vsync
        {
            OFF,
            ON,
            Default
        };

        struct Properties
        {
            std::string title       = "";
            Mode        mode        = Mode::Default;
            bool        resizable   = true;
            Vsync       vsync       = Vsync::Default;
            Extent      extent      = { 1280, 720 };
        };

        Window(const Properties& properties);
        virtual ~Window();

        virtual VkSurfaceKHR create_surface(VkInstance instance);

        virtual bool should_close();

        virtual void close();

        virtual void process_events();

        virtual float get_content_scale_factor() const;

        Extent resize(const Extent& extent);

        const Extent& get_extent() const;

        Mode get_window_mode() const;

        inline const Properties& get_properties() const
        {
            return properties;
        }

        std::vector<const char*> get_required_surface_extensions() const;

    protected:
        Properties properties;

        GLFWwindow* handle = nullptr;
    };
}