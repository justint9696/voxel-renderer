#include "game/camera.hpp"

Camera::Camera(glm::vec3 position, float fov, glm::vec2 viewport)
        : position(position)
        , fov(fov)
        , viewport(viewport) {
    auto target = glm::vec3(0.0f);
    auto direction = glm::normalize(this->position - target);
    this->up = glm::vec3(0.0f, 1.0f, 0.0f);
    this->forward = glm::vec3(0.0f, 0.0f, 1.0f);
    this->right = glm::normalize(glm::cross(up, direction));
    this->view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -1.0f));
}

void Camera::update_view_matrix() {
    this->view =
        glm::lookAt(this->position, this->position + this->forward, this->up);
}
