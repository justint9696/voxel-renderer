#pragma once

#include "renderer/chunk_mesh.hpp"
#include "world/block.hpp"

#include <glm/vec4.hpp>

constexpr size_t CHUNK_WIDTH = 16;
constexpr size_t CHUNK_HEIGHT = 16;
constexpr size_t CHUNK_DEPTH = 16;
constexpr size_t CHUNK_VOLUME = (CHUNK_WIDTH * CHUNK_HEIGHT * CHUNK_DEPTH);

class ChunkSection {
public:
    ChunkMesh mesh;
    glm::vec3 position;

public:
    ChunkSection() = default;
    ~ChunkSection() = default;

    ChunkSection(glm::vec3 position);

    // draws the chunk section
    void render();

    // determines the position of a block from a given index
    glm::vec3 position_from_index(uint32_t idx);

    // determines the index of a block from a given position
    int32_t index_from_position(glm::vec3 position);

private:
    bool is_dirty = true;
    std::vector<BlockType> blocks;

private:
    // prepares the mesh for rendering
    void prepare_mesh();

    // determines if a face is visible within the mesh
    bool is_visible(glm::vec3 position, glm::vec3 normal);

    // adds a block face to the mesh
    void mesh_block_face(glm::vec3 position, glm::vec3 normal, glm::vec4 uv,
                         uint32_t idx, size_t n);

    // returns the maximum height for the given (x,z) coordinates
    uint32_t max_terrain_height(uint32_t x, uint32_t z);
};
