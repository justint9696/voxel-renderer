#include "world/chunk.hpp"

#include "renderer/renderer.hpp"
#include "logger.hpp"
#include "time.hpp"

#include <cmath>

Chunk::Chunk(glm::vec3 position, uint32_t view_distance) :
        position(position), view_distance(view_distance) {
    glm::vec3 pos;
    size_t nchunks = (view_distance * 2) + 1;

    this->perlin.reseed(util::time::now());
    this->sections.reserve(std::pow(nchunks, 2));

    lg::info("Creating {} chunk sections", std::pow(nchunks, 2));
    for (int32_t x = 0; x < nchunks; x++) {
        for (int32_t z = 0; z < nchunks; z++) {
            pos = this->position +
                  glm::vec3(x * CHUNK_WIDTH, 0.0f, z * CHUNK_DEPTH);
            auto& section = this->sections.emplace_back(pos);
            this->generate(section, pos);
        }
    }

    for (auto& section : this->sections) {
        this->prepare_mesh(section);
    }

    lg::info("Chunk spawned at ({}, {}, {})",
             this->position.x, this->position.y, this->position.z);
}

void Chunk::update(float dt) {
    this->mesh_count = 0;
}

void Chunk::tick(float dt) {
    ChunkSection *section = nullptr;
    while (!this->queue.empty()) {
        if (this->mesh_count++ >= MESH_PER_FRAME)
            break;

        ASSERT((section = this->queue.front()));
        this->queue.pop_front();

        if (section->flags & CHUNK_REGEN) {
            this->regenerate(*section);
        } else if (section->flags & CHUNK_DIRTY) {
            section->regen_position = section->position;
            this->prepare_mesh(*section);
        }
    }

    for (size_t i = 0; i < MESH_TIME_MAX; i++)
        this->avg_mesh += this->mesh_time[i];

    this->avg_mesh = (this->avg_mesh / MESH_TIME_MAX) * 1e-6;

    this->mesh_time[this->mesh_idx] = 0;
    this->mesh_idx = ((this->mesh_idx + 1) % MESH_TIME_MAX);
}

void Chunk::render(const Camera& cam) {
    auto& shader = renderer::shader::get("default");
    shader.use();

    renderer::texture::bind("atlas");

    shader.set<glm::mat4>("u_view", cam.view);
    shader.set<glm::mat4>("u_projection", cam.projection);
    shader.set<glm::vec4>("u_color", glm::vec4(1.0f));

    for (auto& section : this->sections) {
        section.render();
    }
}

glm::vec3 Chunk::center(void) const {
    size_t size = ((this->view_distance * 2) + 1);
    return this->position + glm::vec3( 
        ((size * CHUNK_WIDTH) / 2.0f),
        CHUNK_HEIGHT + 5.0f,
        ((size * CHUNK_DEPTH) / 2.0f)
    );
}

bool Chunk::position_in_center(glm::vec3 position) {
    auto center =
        this->position + (glm::vec3(CHUNK_WIDTH, 0.0f, CHUNK_DEPTH) *
                          static_cast<float>(this->view_distance));
    for (auto& section : this->sections) {
        if (section.position == center)
            return section.contains(position);
    }

    return false;
}

void Chunk::swap(glm::vec3 position) {
    auto center =
        this->position + (glm::vec3(CHUNK_WIDTH, 0.0f, CHUNK_DEPTH) *
                          static_cast<float>(this->view_distance));

    // determine the swap direction vector
    auto norm = glm::vec3(0.0f);
    if (position.x < center.x)
        norm.x = -1.0f;
    else if (position.x >= center.x + CHUNK_WIDTH)
        norm.x = 1.0f;
    else if (position.z < center.z)
        norm.z = -1.0f;
    else if (position.z >= center.z + CHUNK_DEPTH)
        norm.z = 1.0f;

    auto vdir = norm * glm::vec3(CHUNK_WIDTH, 0.0f, CHUNK_DEPTH);

    // update the chunk position by direction vector
    this->position += vdir;
    lg::debug("Position updated ({}, {}, {})",
            this->position.x, this->position.y, this->position.z);

    auto dist = static_cast<float>((this->view_distance * 2) + 1);
    auto max =
        (this->position + (glm::vec3(CHUNK_WIDTH, 0.0f, CHUNK_DEPTH) * dist));

    // regenerate the sections that are out of bounds
    ChunkSection *other;
    for (auto& section : this->sections) {
        if (section.position.x < this->position.x ||
            section.position.x >= max.x ||
            section.position.z < this->position.z ||
            section.position.z >= max.z) {
            ASSERT((other = this->section_from_position(section.position + vdir)));
            other->flags |= CHUNK_DIRTY;
            this->queue.push_back(other);

            section.regen_position = section.position;
            section.position += (vdir * dist);
            section.flags |= CHUNK_REGEN;
            this->queue.push_front(&section);

            ASSERT((other = this->section_from_position(section.position - vdir)));
            other->flags |= CHUNK_DIRTY;
            this->queue.push_back(other);
        }
    }
}

void Chunk::prepare_mesh(ChunkSection& section) {
    uint32_t idx = 0;
    size_t n = 0;
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec4 uv;
    time_t start;
    time_t end;

    // destroy the previous mesh
    this->vram -= section.mesh.vram;
    section.mesh.clear();

    start = util::time::now();

    ASSERT(section.blocks.size());
    for (size_t i = 0; i < CHUNK_VOLUME; i++) {
        const auto& type = section.blocks.at(i);
        if (type == BlockType::Air)
            continue;

        idx = static_cast<uint32_t>(type) - 1;
        const auto& block = BLOCK_TABLE[idx];
        position = section.position_from_index(i);

        // only mesh the topmost water block in bodies of water
        if (type == BlockType::Water && position.y < CHUNK_WATER_HEIGHT - 1)
            continue;

        for (size_t j = 0; j < BLOCK_FACES; j++) {
            normal = BLOCK_NORMALS[j];

            // only mesh the top face of water blocks
            if (type == BlockType::Water && normal.y < 1.0f)
                continue;

            if (!section.is_visible(position, normal))
                continue;

            auto other = this->section_from_position(position + normal);
            if (other && !other->is_visible(position, normal))
                continue;

            if (fabs(normal.y) < 1.0f)
                uv = renderer::texture::uv_coords(
                        "atlas", static_cast<uint32_t>(block.front));
            else
                uv = renderer::texture::uv_coords(
                        "atlas", static_cast<uint32_t>(block.top));

            section.mesh_block_face(section.mesh, position - section.position, normal,
                                    uv, block.opacity, j, n++);
        }
    }

    section.mesh.allocate(false);
    section.mesh.submit();

    section.flags &= ~CHUNK_DIRTY;

    this->vram += section.mesh.vram;

    end = util::time::now();
    this->mesh_time[this->mesh_idx] = (end - start);
    this->mesh_idx = ((this->mesh_idx + 1) % MESH_TIME_MAX);
}

ChunkSection *Chunk::section_from_position(glm::vec3 position) {
    ChunkSection *section = nullptr;
    for (auto& tmp : this->sections) {
        if (tmp.contains(position)) {
            section = &tmp;
            break;
        }
    }

    return section;
}

void Chunk::generate(ChunkSection& section, glm::vec3 position) {
    lg::trace("Generating chunk at ({}, {}, {})",
              position.x, position.y, position.z);
    ChunkParams params;

    section.blocks.clear();
    for (size_t x = 0; x < CHUNK_WIDTH; x++) {
        for (size_t z = 0; z < CHUNK_DEPTH; z++) {
            this->params_at(params, position.x + x, position.z + z);
            for (size_t y = 0; y < CHUNK_HEIGHT; y++) {
                if (y >= params.height) {
                    if (y < CHUNK_WATER_HEIGHT) {
                        section.blocks.emplace_back(BlockType::Water);
                    } else {
                        section.blocks.emplace_back(BlockType::Air);
                    }
                } else if (y == params.height - 1) {
                    section.blocks.emplace_back(BlockType::Grass);
                } else if (y >= params.height - 3) {
                    section.blocks.emplace_back(BlockType::Dirt);
                } else {
                    section.blocks.emplace_back(BlockType::Stone);
                }
            }
        }
    }

    section.position = position;
}

void Chunk::regenerate(ChunkSection& section) {
    this->generate(section, section.position);

    section.flags &= ~CHUNK_REGEN;
    section.flags |= CHUNK_DIRTY;

    this->queue.push_back(&section);
}

void Chunk::params_at(ChunkParams& params, float x, float z) {
    float height =
        CHUNK_HEIGHT * this->perlin.octave2D_01(x * 0.01f, z * 0.01f, 3);
    params.height = ceilf(fmaxf(fminf(height, CHUNK_HEIGHT), 1));
}
