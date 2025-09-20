#include "renderer/window.hpp"
#include "logger.hpp"

Window::Window(glm::vec2 size, const std::string& title) : size(size) {
    if ((this->handle = glfwCreateWindow(
                    size.x, size.y, title.c_str(), NULL, NULL)) == NULL) {
        lg::fatal("Failed to create window");
    }

    glfwMakeContextCurrent(this->handle);
}
