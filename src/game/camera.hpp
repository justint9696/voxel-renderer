#pragma once

#include <glm/common.hpp>
#include <glm/ext.hpp>

class Camera {
public:
    glm::mat4 view;
    glm::mat4 projection;
    glm::vec3 position;

public:
    Camera() = default;
    ~Camera() = default;

    Camera(glm::vec3 position, float fov, glm::vec2 viewport) :
        position(position), fov(fov), viewport(viewport) {
            auto target = glm::vec3(0.0f);
            auto direction = glm::normalize(this->position - target);
            auto up = glm::vec3(0.0f, 1.0f, 0.0f);
            this->right = glm::normalize(glm::cross(up, direction));
            this->up = glm::normalize(glm::cross(direction, this->right));
            this->view =
                glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -1.0f));
    }

    void update_view() {
        this->view = glm::lookAt(this->position, glm::vec3(0.0f), this->up);
    }

protected:
    float fov;
    glm::vec2 viewport;
    glm::vec3 up;
    glm::vec3 right;
    glm::vec3 forward;
};

class PerspectiveCamera : public Camera {
public:
    PerspectiveCamera() = default;
    ~PerspectiveCamera() = default;

    PerspectiveCamera(glm::vec3 position, float fov, glm::vec2 viewport) :
        Camera(position, fov, viewport) {
            this->projection = glm::perspective(glm::radians(fov),
                    viewport.x / viewport.y,
                    0.1f, 100.0f);
        }
};
