#pragma once

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <glm/vec2.hpp>

#include <array>
#include <ctime>

enum class KeyState {
    Up, Down, Held
};

struct Key {
    KeyState state = KeyState::Up;
};

struct Mouse {
    glm::vec2 pos = glm::vec2 { 0 };
    glm::vec2 prev = glm::vec2 { 0 };
};

class Input {
public:
    void tick();
    void update_key(GLFWwindow *window, int key, int action);
    bool key_pressed(int key) const;
    bool key_held(int key, time_t delay = 0) const;
    bool key_released(int key) const;

    void mouse_enter(glm::vec2 pos);
    void mouse_update(glm::vec2 pos);
    void mouse_reset();
    glm::vec2 mouse_delta() const;

private:
    std::array<Key, GLFW_KEY_LAST> keys;
    Mouse mouse;
};
