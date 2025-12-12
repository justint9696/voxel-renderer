#pragma once

#include "renderer/chunk_mesh.hpp"
#include "world/block.hpp"

#include <glm/vec4.hpp>

constexpr uint32_t CHUNK_DIRTY = (1 << 0);
constexpr uint32_t CHUNK_REGEN = (1 << 1);

constexpr uint32_t CHUNK_WIDTH = 16;
constexpr uint32_t CHUNK_HEIGHT = 256;
constexpr uint32_t CHUNK_DEPTH = 16;
constexpr uint32_t CHUNK_VOLUME = (CHUNK_WIDTH * CHUNK_HEIGHT * CHUNK_DEPTH);

constexpr uint32_t CHUNK_WATER_HEIGHT = static_cast<uint32_t>(CHUNK_HEIGHT / 2.0f);

struct ChunkParams {
    uint32_t height;
};

class ChunkSection {
public:
    uint32_t flags = 0;
    glm::vec3 position;
    glm::vec3 regen_position;
    ChunkMesh mesh;
    std::vector<BlockType> blocks;

public:
    ChunkSection() = default;
    ~ChunkSection() = default;

    ChunkSection(glm::vec3 position);

    void render(void);

    // Determines the position of a block from a given index
    glm::vec3 position_from_index(uint32_t idx);

    // Determines the index of a block from a given position
    int32_t index_from_position(glm::vec3 position);

    // Determines if a position is contained in the section
    bool contains(glm::vec3 position);

    // Determines if a face is visible within the mesh
    bool is_visible(glm::vec3 position, glm::vec3 normal);

    // Adds a block face to the mesh
    void mesh_block_face(ChunkMesh& mesh, glm::vec3 position, glm::vec3 normal,
                         glm::vec4 uv, float opacity, uint32_t idx, size_t n);
};
