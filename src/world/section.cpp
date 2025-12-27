#include "world/section.hpp"

#include "renderer/renderer.hpp"
#include "logger.hpp"

#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <cmath>

ChunkSection::ChunkSection(glm::vec3 position) :
        position(position), regen_position(position) {
    this->solid.init();
    this->transparent.init();
    this->blocks.reserve(CHUNK_VOLUME);
}

void ChunkSection::render(void) {
    auto& shader = renderer::shader::get("default");
    auto model = glm::translate(glm::mat4(1.0f), this->regen_position);
    shader.set<glm::mat4>("u_model", model);

    this->solid.render();
    this->transparent.render();
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

void ChunkSection::mesh_block_face(ChunkMesh& mesh, glm::vec3 position,
                                   glm::vec3 normal, glm::vec4 uv, float opacity,
                                   uint32_t idx, size_t n) {
    // push the vertices
    for (size_t i = 0; i < 6; i++) {
        // determine the face index
        uint32_t index = ((idx < 3) ? BLOCK_INDICES[i] :
                                      BLOCK_INDICES[i+6]);

        // get the vertex position from the index
        const auto& vertex = BLOCK_VERTICES[index + (idx * 4)];
        const auto& uvs = BLOCK_UVS[index];

        mesh.vertices.emplace_back((ChunkVertex) {
            .position = (position + vertex),
            .uv = {
                ((uvs.x < 1.0f) ? uv.x : uv.z),
                ((uvs.y < 1.0f) ? uv.y : uv.w)
            },
            .normal = normal,
            .opacity = opacity,
        });
    }
}

bool ChunkSection::is_visible(glm::vec3 position, glm::vec3 normal) {
    int32_t idx = this->index_from_position(position + normal);
    if (idx < 0)
        return true;

    auto& type = this->blocks.at(idx);
    auto& block = BLOCK_TABLE[static_cast<uint32_t>(type) - 1];

    return ((type == BlockType::Air) || (block.flags & BLOCK_TRANSPARENT));
}

bool ChunkSection::contains(glm::vec3 position) {
    return ((this->position.x <= position.x &&
             this->position.x + CHUNK_WIDTH > position.x) &&
            (this->position.z <= position.z &&
             this->position.z + CHUNK_DEPTH > position.z));
}
