#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/vec2.hpp>
#include <cstdint>
#include <string>

class Texture {
public:
    Texture() = default;
    ~Texture() = default;

    Texture(const std::string& fname);

    inline void bind() const {
        glBindTexture(GL_TEXTURE_2D, this->handle);
    }

private:
    glm::ivec2 size;
    uint32_t handle;
};
