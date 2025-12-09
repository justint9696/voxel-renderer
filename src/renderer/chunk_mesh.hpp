#pragma once

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <vector>

class ChunkMesh {
public:
    float vram = 0.0f;
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec2> uvs;
    std::vector<glm::vec3> normals;
    std::vector<uint32_t> indices;

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

private:
    uint32_t vao;
    uint32_t vbo;
    uint32_t ibo;
};
