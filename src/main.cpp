#include "core/Application.h"
#include <iostream>
#include <stdexcept>

int main() {
    try {
        clonemine::Application app("CloneMine - A Minecraft Clone", 1280, 720);
        app.run();
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }
    
    return EXIT_SUCCESS;
}
