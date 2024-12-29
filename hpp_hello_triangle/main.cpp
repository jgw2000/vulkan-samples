#include "hpp_hello_triangle.h"

#include <iostream>
#include <stdexcept>

int main()
{
    vks::HppHelloTriangle app{"Hello Triangle", 800, 600};

    try {
        app.Run();
    }
    catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}