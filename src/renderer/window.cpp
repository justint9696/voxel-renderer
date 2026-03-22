#include "renderer/window.hpp"

#include "game/input.hpp"
#include "logger.hpp"

void keyboard_callback(GLFWwindow *glfw_window, int key, int scancode,
                       int action, int mods) {
    if (auto window =
            static_cast<Window *>(glfwGetWindowUserPointer(glfw_window))) {
        window->input.update_key(window->handle, key, action);
    }
}

void mouse_callback(GLFWwindow *glfw_window, double x, double y) {
    static bool init = true;
    if (auto window =
            static_cast<Window *>(glfwGetWindowUserPointer(glfw_window))) {
        if (init) {
            init = false;
            window->input.mouse_enter(glm::vec2{ x, y });
            lg::debug("Mouse entered");
        } else {
            window->input.mouse_update(glm::vec2{ x, y });
        }
    }
}

void window_size_callback(GLFWwindow *glfw_window, int width, int height) {
    if (auto window =
            static_cast<Window *>(glfwGetWindowUserPointer(glfw_window))) {
        window->size = { width, height };
    }
    glViewport(0, 0, width, height);
}

Window::Window(glm::ivec2 size, std::string_view title)
        : size(size) {
    if (!glfwInit()) {
        lg::fatal("Failed to initialize GLFW");
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

    if (!(this->handle = glfwCreateWindow(
                    size.x, size.y, title.data(), nullptr, nullptr))) {
        lg::fatal("Failed to create window");
    }

    glfwMakeContextCurrent(this->handle);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        lg::fatal("Failed to initialize GLAD");
    }

    glfwSetWindowUserPointer(this->handle, this);
    glfwSetInputMode(this->handle, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(this->handle, mouse_callback);
    glfwSetKeyCallback(this->handle, keyboard_callback);
    glfwSetFramebufferSizeCallback(this->handle, window_size_callback);

    // disable vsync
    glfwSwapInterval(0);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glViewport(0, 0, size.x, size.y);
}
