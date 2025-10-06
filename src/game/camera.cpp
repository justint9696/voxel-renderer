#include "game/camera.hpp"

#include "game/input.hpp"

constexpr float CAMERA_SPEED = 15.0f;
constexpr float CAMERA_SENSITIVITY = 3.0f;

Camera::Camera(glm::vec3 position, float fov, glm::vec2 viewport) :
    position(position), fov(fov), viewport(viewport) {
        auto target = glm::vec3(0.0f);
        auto direction = glm::normalize(this->position - target);
        this->up = glm::vec3(0.0f, 1.0f, 0.0f);
        this->forward = glm::vec3(0.0f, 0.0f, 1.0f);
        this->right = glm::normalize(glm::cross(up, direction));
        this->view =
                glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -1.0f));
}

void Camera::tick(float dt) {
        if (input::key_held(GLFW_KEY_W)) {
            this->position += this->forward * CAMERA_SPEED * dt;
        }
        else if (input::key_held(GLFW_KEY_S)) {
            this->position -= this->forward * CAMERA_SPEED * dt;
        }
        if (input::key_held(GLFW_KEY_A)) {
            auto right = glm::normalize(glm::cross(up, this->forward));
            this->position += right * CAMERA_SPEED * dt;
        }
        else if (input::key_held(GLFW_KEY_D)) {
            auto right = glm::normalize(glm::cross(up, this->forward));
            this->position -= right * CAMERA_SPEED * dt;
        }

        glm::vec2 delta = input::mouse::delta();
        this->yaw += delta.x * CAMERA_SENSITIVITY * dt;
        this->pitch -= delta.y * CAMERA_SENSITIVITY * dt;
        this->pitch = glm::clamp(this->pitch, -89.0f, 89.0f);

        this->forward = {
            cos(glm::radians(this->yaw)) * cos(glm::radians(this->pitch)),
            sin(glm::radians(this->pitch)),
            sin(glm::radians(this->yaw)) * cos(glm::radians(this->pitch))
        };

        this->update_view_matrix();
        input::mouse::reset();
}

void Camera::update_view_matrix() {
    this->view = glm::lookAt(
            this->position, this->position + this->forward, this->up);
}
