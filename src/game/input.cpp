#include "game/input.hpp"
#include "time.hpp"

constexpr time_t KEY_HOLD_MS = 125;
constexpr time_t KEY_HOLD_NS = (KEY_HOLD_MS * 1e+6);

void Input::update_key(GLFWwindow *window, int key, int action) {
    auto& k = this->keys.at(key);
    auto tick = util::time::now();
    switch (action) {
        case GLFW_PRESS:
            k.state = KeyState::Down;
            k.pressed_tick = tick;
            break;
        case GLFW_RELEASE:
            k.state = KeyState::Up;
            k.released_tick = tick;
            break;
        default:
            break;
    }
}

bool Input::key_pressed(int key) const {
    auto tick = util::time::now();
    const auto& k = this->keys.at(key);
    switch (k.state) {
        case KeyState::Down:
            return (tick - k.pressed_tick) < KEY_HOLD_NS;
        default:
            return false;
    }
}

bool Input::key_held(int key, time_t delay) const {
    auto tick = util::time::now();
    const auto& k = this->keys.at(key);
    switch (k.state) {
        case KeyState::Down:
            return (tick - k.pressed_tick) > delay;
        default:
            return false;
    }
}

bool Input::key_released(int key) const {
    switch (this->keys.at(key).state) {
        case KeyState::Up:
            return true;
        default:
            return false;
    }
}

void Input::mouse_enter(glm::vec2 pos) {
    this->mouse.pos = pos;
    this->mouse.prev = pos;
}

void Input::mouse_update(glm::vec2 pos) {
    this->mouse_reset();
    this->mouse.pos = pos;
}

void Input::mouse_reset() {
    this->mouse.prev = this->mouse.pos;
}

glm::vec2 Input::mouse_delta() const {
    return this->mouse.pos - this->mouse.prev;
}
