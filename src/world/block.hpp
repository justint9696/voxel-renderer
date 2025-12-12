#pragma once

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <cstddef>
#include <cstdint>

constexpr size_t BLOCK_FACES = 6;

constexpr uint32_t BLOCK_SOLID = (1 << 0);
constexpr uint32_t BLOCK_ANIMATED = (1 << 1);
constexpr uint32_t BLOCK_TRANSPARENT = (1 << 2);

enum class BlockType {
    Air = 0,
    Grass,
    Dirt,
    Stone,
    Water,
};

enum class BlockTexture {
    Grass0 = 0,
    Grass1,
    Dirt0,
    Stone0,
    Water0 = 16,
};

struct Block {
    BlockTexture top;
    BlockTexture front;
    uint32_t flags;
    uint32_t frames;
    float opacity;
};

extern const Block BLOCK_TABLE[];

extern const glm::vec3 BLOCK_VERTICES[];
extern const glm::vec2 BLOCK_UVS[];
extern const glm::vec3 BLOCK_NORMALS[];
extern const uint32_t BLOCK_INDICES[];
