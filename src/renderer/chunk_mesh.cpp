#include "renderer/chunk_mesh.hpp"
#include "logger.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

void ChunkMesh::init() {
    glGenVertexArrays(1, &this->vao);
    glGenBuffers(1, &this->vbo);

    glBindVertexArray(this->vao);
    glBindBuffer(GL_ARRAY_BUFFER, this->vbo);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(ChunkVertex),
                          (void *)offsetof(ChunkVertex, position));
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(ChunkVertex),
                          (void *)offsetof(ChunkVertex, uv));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(ChunkVertex),
                          (void *)offsetof(ChunkVertex, normal));
    glEnableVertexAttribArray(2);

    glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(ChunkVertex),
                          (void *)offsetof(ChunkVertex, opacity));
    glEnableVertexAttribArray(3);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void ChunkMesh::allocate(bool dynamic) {
    size_t nbytes = sizeof(ChunkVertex) * this->vertices.size();
    
    glBindBuffer(GL_ARRAY_BUFFER, this->vbo);
    glBufferData(GL_ARRAY_BUFFER, nbytes, nullptr,
                 ((dynamic) ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW));
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    this->vram = static_cast<float>(nbytes * 1e-6);
    lg::trace("Allocated {:.2f} MB for mesh", this->vram);
}

void ChunkMesh::submit() {
    glBindVertexArray(this->vao);
    glBindBuffer(GL_ARRAY_BUFFER, this->vbo);

    glBufferSubData(GL_ARRAY_BUFFER, 0,
                    sizeof(ChunkVertex) * this->vertices.size(),
                    this->vertices.data());

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    lg::debug("Created mesh from {} vertices", this->vertices.size());
}

void ChunkMesh::render() {
    glBindVertexArray(this->vao);
    glDrawArrays(GL_TRIANGLES, 0, this->vertices.size());
    glBindVertexArray(0);
}

void ChunkMesh::clear() {
    this->vertices.clear();
}

void ChunkMesh::sort(const Camera& cam) {
}
