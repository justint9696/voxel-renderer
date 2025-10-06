#include "world/section.hpp"

#include "renderer/renderer.hpp"
#include "world/block.hpp"
#include "logger.hpp"

#include <glm/ext.hpp>
#include <cmath>

ChunkSection::ChunkSection(glm::vec3 position) : position(position) {
    uint32_t height = 0;
    this->blocks.reserve(CHUNK_VOLUME);
    for (size_t x = 0; x < CHUNK_WIDTH; x++) {
        for (size_t z = 0; z < CHUNK_DEPTH; z++) {
            height = this->max_terrain_height(x, z);
            for (size_t y = 0; y < height; y++) {
                this->blocks.emplace_back(BlockType::Grass);
            }
        }
    }

    this->mesh.init();
    this->prepare_mesh();
}

void ChunkSection::render() {
    auto& shader = renderer::shader::get("default");
    auto model = glm::translate(glm::mat4(1.0f), this->position);
    // model = glm::rotate(model,
    //                 static_cast<float>(glfwGetTime() * glm::radians(45.0f)),
    //                 glm::vec3(0.5f, 0.5f, 0.0f));
    shader.set<glm::mat4>("u_model", model);

    if (this->is_dirty) {
        this->prepare_mesh();
    }

    this->mesh.render();
}

void ChunkSection::prepare_mesh() {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec4 uv;

    // destroy the previous mesh
    this->mesh.clear();

    for (size_t i = 0; i < CHUNK_VOLUME; i++) {
        const auto& block = this->blocks.at(i);
        if (block == BlockType::Air)
            continue;

        position = this->position_from_index(i);
        uv = renderer::texture::uv_coords(
                "atlas", static_cast<uint32_t>(block) - 1);
        for (size_t j = 0; j < BLOCK_FACES; j++) {
            normal = BLOCK_NORMALS[j];
            this->mesh_block_face(position, normal, uv, j, i);
        }
    }

    this->mesh.allocate(false);
    this->mesh.submit();

    this->is_dirty = false;
}

inline glm::vec3 ChunkSection::position_from_index(uint32_t idx) {
    if (idx >= CHUNK_VOLUME) {
        lg::fatal("Index out of bounds: {}", idx);
    }

    return {
        floorf(1.0f * idx / (CHUNK_HEIGHT * CHUNK_DEPTH)),
        idx % CHUNK_HEIGHT,
        fmodf(floorf(1.0f * idx / CHUNK_HEIGHT), CHUNK_DEPTH)
    };
}

uint32_t ChunkSection::max_terrain_height(uint32_t x, uint32_t z) {
    return CHUNK_HEIGHT;
}

void ChunkSection::mesh_block_face(glm::vec3 position, glm::vec3 normal,
                                   glm::vec4 uv, uint32_t idx, size_t count) {
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

    if (!this->is_visible(position, normal))
        return;

    // push the indices
    for (size_t i = 0; i < 6; i++) {
        this->mesh.indices.emplace_back(
                BLOCK_INDICES[i] + (count * BLOCK_FACES * 4) + (idx * 4));
    }
}

inline bool ChunkSection::is_visible(glm::vec3 position, glm::vec3 normal) {
    return true;
}
