#include <iostream>
#include <stdexcept>

#include "vulkan_sample.h"

int main()
{
    vkb::Window::Properties properties;
    properties.title = "Hello World";

    vkb::VulkanSample sample;
    sample.run(properties);

    return EXIT_SUCCESS;
}