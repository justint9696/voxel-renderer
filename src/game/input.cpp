#include "game/input.hpp"

void Input::tick() {
    for (auto& key : this->keys) {
        switch (key.state) {
            case KeyState::Down:
                key.state = KeyState::Held;
                break;
            default:
                break;
        }
    }
}

void Input::update_key(GLFWwindow *window, int key, int action) {
    auto& k = this->keys.at(key);
    switch (action) {
        case GLFW_PRESS:
            k.state = KeyState::Down;
            break;
        case GLFW_RELEASE:
            k.state = KeyState::Up;
            break;
        default:
            break;
    }
}

bool Input::key_pressed(int key) const {
    const auto& k = this->keys.at(key);
    switch (k.state) {
        case KeyState::Down:
            return true;
        default:
            return false;
    }
}

bool Input::key_held(int key, time_t delay) const {
    const auto& k = this->keys.at(key);
    switch (k.state) {
        case KeyState::Held:
            return true;
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
