#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/vec2.hpp>
#include <glm/vec4.hpp>
#include <string>

class Texture {
public:
    Texture() = default;
    ~Texture() = default;

    Texture(const std::string& fname, glm::ivec2 size);

    inline void bind() const {
        glBindTexture(GL_TEXTURE_2D, this->handle);
    }

    // return uv coords from a cell position
    glm::vec4 uv_coords(glm::vec2 pos);

    // return uv coords from a cell index
    glm::vec4 uv_coords(uint32_t idx);

private:
    glm::ivec2 size;
    glm::ivec2 dim;
    uint32_t handle;
};
