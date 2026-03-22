#include "player.hpp"

#include <glm/glm.hpp>

void Player::tick(const Input& input, Camera& camera, float dt) {
    if (input.key_held(GLFW_KEY_W)) {
        camera.position += camera.forward * CAMERA_SPEED * dt;
    }
    else if (input.key_held(GLFW_KEY_S)) {
        camera.position -= camera.forward * CAMERA_SPEED * dt;
    }
    if (input.key_held(GLFW_KEY_A)) {
        auto right = glm::normalize(glm::cross(camera.up, camera.forward));
        camera.position += right * CAMERA_SPEED * dt;
    }
    else if (input.key_held(GLFW_KEY_D)) {
        auto right = glm::normalize(glm::cross(camera.up, camera.forward));
        camera.position -= right * CAMERA_SPEED * dt;
    }

    glm::vec2 delta = input.mouse_delta();
    camera.yaw += delta.x * CAMERA_SENSITIVITY * dt;
    camera.pitch -= delta.y * CAMERA_SENSITIVITY * dt;
    camera.pitch = glm::clamp(camera.pitch, -89.0f, 89.0f);

    camera.forward = {
        cos(glm::radians(camera.yaw)) * cos(glm::radians(camera.pitch)),
        sin(glm::radians(camera.pitch)),
        sin(glm::radians(camera.yaw)) * cos(glm::radians(camera.pitch))
    };

    camera.update_view_matrix();
}
