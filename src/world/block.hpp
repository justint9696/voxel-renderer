#pragma once

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <cstddef>
#include <cstdint>

constexpr size_t BLOCK_FACES = 6;

enum class BlockType {
    Air = 0,
    Grass,
};

const glm::vec3 BLOCK_VERTICES[] = {
    { 0.0f, 0.0f, 1.0f }, // front
    { 0.0f, 1.0f, 1.0f },
    { 1.0f, 1.0f, 1.0f },
    { 1.0f, 0.0f, 1.0f },

    { 0.0f, 0.0f, 0.0f }, // back
    { 0.0f, 1.0f, 0.0f },
    { 1.0f, 1.0f, 0.0f },
    { 1.0f, 0.0f, 0.0f },

    { 0.0f, 1.0f, 0.0f }, // top
    { 0.0f, 1.0f, 1.0f },
    { 1.0f, 1.0f, 1.0f },
    { 1.0f, 1.0f, 0.0f },

    { 0.0f, 0.0f, 0.0f }, // bottom
    { 0.0f, 0.0f, 1.0f },
    { 1.0f, 0.0f, 1.0f },
    { 1.0f, 0.0f, 0.0f },

    { 0.0f, 0.0f, 0.0f }, // left
    { 0.0f, 1.0f, 0.0f },
    { 0.0f, 1.0f, 1.0f },
    { 0.0f, 0.0f, 1.0f },

    { 1.0f, 0.0f, 0.0f }, // right
    { 1.0f, 1.0f, 0.0f },
    { 1.0f, 1.0f, 1.0f },
    { 1.0f, 0.0f, 1.0f },
};

const glm::vec2 BLOCK_UVS[] = {
    { 0.0f, 0.0f },
    { 0.0f, 1.0f },
    { 1.0f, 1.0f },
    { 1.0f, 0.0f },
};

const glm::vec3 BLOCK_NORMALS[] = {
    { 0.0f, 0.0f, +1.0f }, // front
    { 0.0f, 0.0f, -1.0f }, // back
    { 0.0f, +1.0f, 0.0f }, // top
    { 0.0f, -1.0f, 0.0f }, // bottom
    { -1.0f, 0.0f, 0.0f }, // left
    { +1.0f, 0.0f, 0.0f }, // right
};

const uint32_t BLOCK_INDICES[] = {
    0, 1, 3,
    1, 2, 3
};
