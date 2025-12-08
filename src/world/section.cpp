#include "world/section.hpp"

#include "renderer/renderer.hpp"
#include "logger.hpp"

#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <cmath>

ChunkSection::ChunkSection(glm::vec3 position) : position(position) {
    this->mesh.init();
    this->blocks.reserve(CHUNK_VOLUME);
}

void ChunkSection::render(void) {
    auto& shader = renderer::shader::get("default");
    auto model = glm::translate(glm::mat4(1.0f), this->position);
    shader.set<glm::mat4>("u_model", model);

    this->mesh.render();
}

glm::vec3 ChunkSection::position_from_index(uint32_t idx) {
    if (idx >= CHUNK_VOLUME) {
        lg::fatal("Index out of bounds: {}", idx);
    }

    return this->position + glm::vec3(
        floorf(1.0f * idx / (CHUNK_HEIGHT * CHUNK_DEPTH)),
        idx % CHUNK_HEIGHT,
        fmodf(floorf(1.0f * idx / CHUNK_HEIGHT), CHUNK_DEPTH)
    );
}

int32_t ChunkSection::index_from_position(glm::vec3 position) {
    int32_t idx;

    position -= this->position;
    if ((position.x < 0 || position.x >= CHUNK_WIDTH) || 
        (position.y < 0 || position.y >= CHUNK_HEIGHT) ||
        (position.z < 0 || position.z >= CHUNK_DEPTH))
        return -1;

    idx = ((position.x * (CHUNK_HEIGHT * CHUNK_DEPTH)) +
           (position.y) +
           (position.z * CHUNK_HEIGHT));

    if (idx >= CHUNK_VOLUME || idx < 0)
        return -1;

    return idx;
}

uint32_t ChunkSection::max_terrain_height(uint32_t x, uint32_t z) {
    return CHUNK_HEIGHT;
}

void ChunkSection::mesh_block_face(glm::vec3 position, glm::vec3 normal,
                                   glm::vec4 uv, uint32_t idx, size_t n) {
    // push the vertices
    for (size_t i = 0; i < 4; i++) {
        const auto& vertex = BLOCK_VERTICES[i + (idx * 4)];
        this->mesh.vertices.emplace_back(position + vertex);
    }

    // push the uv coordinates
    for (size_t i = 0; i < 4; i++) {
        const auto& uvs = BLOCK_UVS[i];
        this->mesh.uvs.emplace_back(
            ((uvs.x < 1.0f) ? uv.x : uv.z),
            ((uvs.y < 1.0f) ? uv.y : uv.w)
        );
    }

    // push the normals
    for (size_t i = 0; i < 4; i++) {
        this->mesh.normals.emplace_back(normal);
    }

    // push the indices
    size_t lo = ((idx < 3) ? 0 : 6);
    size_t hi = ((idx < 3) ? 6 : 12);
    for (size_t i = lo; i < hi; i++) {
        this->mesh.indices.emplace_back(BLOCK_INDICES[i] + (n * 4));
    }
}

bool ChunkSection::is_visible(glm::vec3 position, glm::vec3 normal) {
    int32_t idx = this->index_from_position(position + normal);
    if (idx < 0)
        return true;

    auto& type = this->blocks.at(idx);
    auto& block = BLOCK_TABLE[static_cast<uint32_t>(type)];

    return ((type == BlockType::Air) || (block.flags & BLOCK_TRANSPARENT));
}

bool ChunkSection::contains(glm::vec3 position) {
    return ((this->position.x <= position.x &&
             this->position.x + CHUNK_WIDTH > position.x) &&
            (this->position.z <= position.z &&
             this->position.z + CHUNK_DEPTH > position.z));
}
