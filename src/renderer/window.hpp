#pragma once

#include "game/input.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/vec2.hpp>
#include <glm/vec4.hpp>
#include <format>
#include <string_view>

class Window {
public:
    glm::vec2 size;
    GLFWwindow *handle = nullptr;
    Input input;

public:
    Window(glm::ivec2 size, std::string_view title);

    template <typename... Args>
    void set_title(const std::string& fmt, Args&&... args) const {
        auto title = std::vformat(fmt, std::make_format_args(args...));
        glfwSetWindowTitle(this->handle, title.c_str());
    }

    inline bool should_close() const {
        return glfwWindowShouldClose(this->handle);
    }

    inline void close() const {
        glfwSetWindowShouldClose(this->handle, GLFW_TRUE);
        glfwDestroyWindow(this->handle);
    }

    inline void swap() const {
        glfwSwapBuffers(this->handle);
    }

    inline void poll() const {
        glfwPollEvents();
    }

    inline void clear() const {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    inline void clear_color(float r, float g, float b, float a) const {
        glClearColor(r, g, b, a);
    }
};
