#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/vec2.hpp>
#include <format>
#include <string>

class Window {
public:
    glm::vec2 size;
    GLFWwindow *handle = nullptr;

public:
    Window() = default;
    ~Window() = default;

    Window(glm::vec2 size, const std::string& title);

    template <typename... Args>
    void set_title(const std::string& fmt, Args&&... args) const {
        auto title = std::vformat(fmt, std::make_format_args(args...));
        glfwSetWindowTitle(this->handle, title.c_str());
    }

    inline bool is_open() const {
        return !glfwWindowShouldClose(this->handle);
    }

    inline void close() const {
        glfwSetWindowShouldClose(this->handle, GLFW_TRUE);
        glfwDestroyWindow(this->handle);
    }
};
