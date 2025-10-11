#include "world/block.hpp"

const Block BLOCK_TABLE[] = {
    { BlockTexture::Grass0, BlockTexture::Grass1, BLOCK_SOLID, 0, 1.0f },
    { BlockTexture::Dirt0, BlockTexture::Dirt0, BLOCK_SOLID, 0, 1.0f },
    { BlockTexture::Stone0, BlockTexture::Stone0, BLOCK_SOLID, 0, 1.0f },
};

const glm::vec3 BLOCK_VERTICES[] = {
    { 0.0f, 0.0f, 0.0f }, // front
    { 0.0f, 1.0f, 0.0f },
    { 1.0f, 1.0f, 0.0f },
    { 1.0f, 0.0f, 0.0f },

    { 0.0f, 1.0f, 0.0f }, // top
    { 0.0f, 1.0f, 1.0f },
    { 1.0f, 1.0f, 1.0f },
    { 1.0f, 1.0f, 0.0f },

    { 1.0f, 0.0f, 0.0f }, // right
    { 1.0f, 1.0f, 0.0f },
    { 1.0f, 1.0f, 1.0f },
    { 1.0f, 0.0f, 1.0f },

    { 0.0f, 0.0f, 1.0f }, // back
    { 0.0f, 1.0f, 1.0f },
    { 1.0f, 1.0f, 1.0f },
    { 1.0f, 0.0f, 1.0f },

    { 0.0f, 0.0f, 0.0f }, // bottom
    { 0.0f, 0.0f, 1.0f },
    { 1.0f, 0.0f, 1.0f },
    { 1.0f, 0.0f, 0.0f },

    { 0.0f, 0.0f, 0.0f }, // left
    { 0.0f, 1.0f, 0.0f },
    { 0.0f, 1.0f, 1.0f },
    { 0.0f, 0.0f, 1.0f },
};

const glm::vec2 BLOCK_UVS[] = {
    { 0.0f, 0.0f },
    { 0.0f, 1.0f },
    { 1.0f, 1.0f },
    { 1.0f, 0.0f },
};

const glm::vec3 BLOCK_NORMALS[] = {
    { 0.0f, 0.0f, -1.0f }, // front
    { 0.0f, +1.0f, 0.0f }, // top
    { +1.0f, 0.0f, 0.0f }, // right
    { 0.0f, 0.0f, +1.0f }, // back
    { 0.0f, -1.0f, 0.0f }, // bottom
    { -1.0f, 0.0f, 0.0f }, // left
};

const uint32_t BLOCK_INDICES[] = {
    0, 1, 3, 1, 2, 3, // front faces
    0, 3, 1, 3, 2, 1  // back faces
};
