#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/vec2.hpp>
#include <string>

class Window {
public:
    glm::vec2 size;
    GLFWwindow *handle = nullptr;

public:
    Window() = default;
    ~Window() {
    }

    Window(glm::vec2 size, const std::string& title);

    inline bool is_open() const {
        return !glfwWindowShouldClose(this->handle);
    }

    inline void close() const {
        glfwSetWindowShouldClose(this->handle, GLFW_TRUE);
        glfwDestroyWindow(this->handle);
    }
};
