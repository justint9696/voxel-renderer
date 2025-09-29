#pragma once

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <vector>

class ChunkMesh {
public:
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec2> uvs;
    std::vector<glm::vec3> normals;
    std::vector<uint32_t> indices;

public:
    ChunkMesh() = default;
    ~ChunkMesh() = default;

    // initialize vertex buffers
    void init();

    // allocates gpu memory from the current buffer sizes
    void allocate(bool dynamic = false);

    // sends vertex data to the gpu
    void submit();

    // renders the vertex data
    void render();

private:
    uint32_t vao;
    uint32_t vbo;
    uint32_t ibo;
};
