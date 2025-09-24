#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <ctime>

namespace input {
    void update_key(GLFWwindow *window, int key, int action, time_t tick);
    bool key_pressed(int key);
    bool key_held(int key, time_t delay = 0);
    bool key_released(int key);
}
