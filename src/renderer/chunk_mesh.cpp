#include "renderer/chunk_mesh.hpp"
#include "logger.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

void ChunkMesh::init() {
    glGenVertexArrays(1, &this->vao);
    glGenBuffers(1, &this->vbo);
    glGenBuffers(1, &this->ibo);
}

void ChunkMesh::allocate(bool dynamic) {
    size_t nbytes = 0;
    nbytes += sizeof(glm::vec3) * this->vertices.size();
    nbytes += sizeof(glm::vec2) * this->uvs.size();
    nbytes += sizeof(glm::vec3) * this->normals.size();
    
    glBindBuffer(GL_ARRAY_BUFFER, this->vbo);
    glBufferData(GL_ARRAY_BUFFER, nbytes, nullptr,
                 ((dynamic) ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW));
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    lg::debug("Allocated {} bytes for mesh", nbytes);
}

void ChunkMesh::submit() {
    size_t size = 0;
    off_t offset = 0;

    glBindVertexArray(this->vao);
    glBindBuffer(GL_ARRAY_BUFFER, this->vbo);

    size = this->vertices.size() * sizeof(glm::vec3);
    glBufferSubData(GL_ARRAY_BUFFER, offset, size, this->vertices.data());
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3),
                          (void *)offset);
    glEnableVertexAttribArray(0);

    offset += size;
    size = this->uvs.size() * sizeof(glm::vec2);
    glBufferSubData(GL_ARRAY_BUFFER, offset, size, this->uvs.data());
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2),
                          (void *)offset);
    glEnableVertexAttribArray(1);

    offset += size;
    size = this->normals.size() * sizeof(glm::vec3);
    glBufferSubData(GL_ARRAY_BUFFER, offset, size, this->normals.data());
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3),
                          (void *)offset);
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                 this->indices.size() * sizeof(uint32_t),
                 this->indices.data(),
                 GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    lg::debug("Created mesh from {} vertices, {} uvs, {} normals, and {} indices",
              this->vertices.size(), 
              this->uvs.size(),
              this->normals.size(),
              this->indices.size());
}

void ChunkMesh::render() {
    glBindVertexArray(this->vao);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->ibo);
    glDrawElements(GL_TRIANGLES, this->indices.size(), GL_UNSIGNED_INT, 0);
}
