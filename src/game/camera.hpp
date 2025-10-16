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

    Camera(glm::vec3 position, float fov, glm::vec2 viewport);

    void tick(float dt);

    void update_view_matrix();

    const glm::vec2& get_viewport() const {
        return this->viewport;
    }

protected:
    float fov;
    float yaw = -90.0f;
    float pitch = 0.0f;
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
        this->projection = glm::perspective(
                glm::radians(fov),
                viewport.x / viewport.y,
                0.1f, 100.0f);
    }
};

class OrthoCamera : public Camera {
public:
    OrthoCamera() = default;
    ~OrthoCamera() = default;

    OrthoCamera(glm::vec2 viewport) :
            Camera(glm::vec3(0.0f), 90.0f, viewport) {
        this->projection = glm::ortho(0.0f, viewport.x, 0.0f, viewport.y,
                                      -100.0f, 100.0f);
    }
};
