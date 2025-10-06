#include "game/input.hpp"
#include "time.hpp"

#include <map>

namespace input {
    enum class KeyState {
        Up,
        Down,
        Held
    };

    struct Key {
        input::KeyState state = input::KeyState::Up;
        time_t pressed_tick = 0;
        time_t released_tick = 0;
    };

    struct Mouse {
        glm::vec2 prev;
        glm::vec2 pos;
    };

    static std::map<int, input::Key> g_keys;

    static Mouse g_mouse;

    static inline input::Key& try_get(int key) {
        input::Key k;
        if (!g_keys.contains(key)) {
            g_keys.insert(std::make_pair(key, k));
        }

        return g_keys.at(key);
    }

    void update_key(GLFWwindow *window, int key, int action, time_t tick) {
        auto& k = input::try_get(key);
        switch (action) {
            case GLFW_PRESS:
                k.state = input::KeyState::Down;
                g_keys.at(key).pressed_tick = tick;
                break;
            case GLFW_RELEASE:
                k.state = input::KeyState::Up;
                g_keys.at(key).released_tick = tick;
                break;
            default:
                break;
        }
    }

    bool key_pressed(int key) {
        auto& k = input::try_get(key);
        switch (k.state) {
            case input::KeyState::Down:
                k.state = input::KeyState::Held;
                return true;
            default:
                return false;
        }
    }

    bool key_held(int key, time_t delay) {
        const auto& k = input::try_get(key);
        const time_t now = util::time::now();
        return ((k.state == input::KeyState::Down ||
                 k.state == input::KeyState::Held) &&
                (now - k.pressed_tick) >= delay);
    }

    bool key_released(int key) {
        const auto& k = input::try_get(key);
        return k.state == input::KeyState::Up;
    }

    namespace mouse {
        void init(glm::vec2 pos) {
            g_mouse.prev = pos;
            g_mouse.pos = pos;
        }

        void update(glm::vec2 pos) {
            g_mouse.prev = g_mouse.pos;
            g_mouse.pos = pos;
        }

        glm::vec2 delta(void) {
            return g_mouse.pos - g_mouse.prev;
        }

        void reset(void) {
            g_mouse.prev = g_mouse.pos;
        }
    }
}
