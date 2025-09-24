#include "renderer/window.hpp"
#include "game/input.hpp"
#include "logger.hpp"
#include "time.hpp"

void keyboard_callback(GLFWwindow *window, int key, int scancode, int action,
                       int mods);

Window::Window(glm::vec2 size, const std::string& title) : size(size) {
    if ((this->handle = glfwCreateWindow(
                    size.x, size.y, title.c_str(), NULL, NULL)) == NULL) {
        lg::fatal("Failed to create window");
    }

    glfwMakeContextCurrent(this->handle);
    glfwSetKeyCallback(this->handle, keyboard_callback);
}

void keyboard_callback(GLFWwindow *window, int key, int scancode, int action,
                       int mods) {
    input::update_key(window, key, action, util::time::now());
}
