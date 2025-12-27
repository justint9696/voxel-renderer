#pragma once

#include "game/camera.hpp"

#include <vector>

struct ChunkVertex {
    glm::vec3 position;
    glm::vec2 uv;
    glm::vec3 normal;
    float opacity;
};

class ChunkMesh {
public:
    float vram = 0.0f;
    std::vector<ChunkVertex> vertices;

public:
    ChunkMesh() = default;
    ~ChunkMesh() = default;

    // Initializes the vertex buffers
    void init();

    // Allocates gpu memory from the current buffer sizes
    void allocate(bool dynamic = false);

    // Sends vertex data to the gpu
    void submit();

    // Renders the vertex data
    void render();

    // Clears the chunk mesh
    void clear();

    // Sorts the mesh data
    void sort(const Camera& cam);

private:
    uint32_t vao;
    uint32_t vbo;
};
