#include "world/chunk.hpp"

#include "logger.hpp"
#include "time.hpp"

#include <cmath>

Chunk::Chunk(glm::vec3 position, uint32_t view_distance)
        : view_distance(view_distance), position(position) {
    glm::vec3 pos;
    size_t nchunks = (view_distance * 2) + 1;
    size_t squared = std::pow(nchunks, 2);

    this->perlin.reseed(util::time::now());
    this->sections.reserve(squared);
    this->indices.resize(squared);

    lg::info("Creating {} chunk sections", squared);
    for (size_t x = 0, idx = 0; x < nchunks; x++) {
        for (size_t z = 0; z < nchunks; z++, idx++) {
            pos = this->position +
                  glm::vec3(x * CHUNK_WIDTH, 0.0f, z * CHUNK_DEPTH);
            auto& section = this->sections.emplace_back(pos);
            section.idx = idx;
            this->generate(section, pos);
            this->indices[idx] = idx;
        }
    }

    for (auto& section : this->sections) {
        section.flags |= CHUNK_DIRTY;
        this->queue.push_back(section.idx);
    }

    this->flags |= CHUNK_SORT;

    lg::info("Chunk spawned at ({}, {}, {})",
             this->position.x, this->position.y, this->position.z);
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
            this->queue.push_back(other->idx);

            section.regen_position = section.position;
            section.position += (vdir * dist);
            section.flags |= CHUNK_REGEN;
            this->queue.push_front(section.idx);

            ASSERT((other = this->section_from_position(section.position - vdir)));
            other->flags |= CHUNK_DIRTY;
            this->queue.push_back(other->idx);
        }
    }

    // notify the renderer to sort the chunks
    this->flags |= CHUNK_SORT;
}

ChunkSection *Chunk::section_from_position(glm::vec3 position) {
    ChunkSection *ret = nullptr;
    for (auto& section : this->sections) {
        if (section.contains(position)) {
            ret = &section;
            break;
        }
    }

    return ret;
}

void Chunk::generate(ChunkSection& section, glm::vec3 position) {
    lg::trace("Generating chunk at ({}, {}, {})",
              position.x, position.y, position.z);
    size_t idx = 0;
    ChunkParams params;

    section.blocks.clear();
    section.indices.clear();
    for (size_t x = 0; x < CHUNK_WIDTH; x++) {
        for (size_t z = 0; z < CHUNK_DEPTH; z++) {
            this->params_at(params, position.x + x, position.z + z);
            for (size_t y = 0; y < CHUNK_HEIGHT; y++, idx++) {
                if (y >= params.height) {
                    if (y < CHUNK_WATER_HEIGHT) {
                        section.blocks.emplace_back(BlockType::Water);
                    } else {
                        section.blocks.emplace_back(BlockType::Air);
                    }
                } else if (y < CHUNK_WATER_HEIGHT + 3) {
                    section.blocks.emplace_back(BlockType::Sand);
                } else if (y == params.height - 1) {
                    section.blocks.emplace_back(BlockType::Grass);
                } else if (y >= params.height - 3) {
                    section.blocks.emplace_back(BlockType::Dirt);
                } else {
                    section.blocks.emplace_back(BlockType::Stone);
                }

                const auto& block = section.blocks.at(idx);
                switch (block) {
                    case BlockType::Air:
                        // no need to mesh blocks of air!
                        break;
                    default:
                        section.indices.emplace_back(idx);
                        break;
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

    this->queue.push_back(section.idx);
}

void Chunk::params_at(ChunkParams& params, float x, float z) {
    float height =
        CHUNK_HEIGHT * this->perlin.octave2D_01(x * 0.01f, z * 0.01f, 3);
    params.height = ceilf(fmaxf(fminf(height, CHUNK_HEIGHT), 1));
}

void Chunk::sort(const Camera& camera) {
    std::sort(
        this->indices.begin(), this->indices.end(),
        [&camera, this](const uint32_t& lval, const uint32_t& rval) {
            const auto& a = this->sections.at(lval);
            const auto& b = this->sections.at(rval);
            return glm::distance(camera.position, a.center()) >
                   glm::distance(camera.position, b.center());
        });
    this->flags &= ~CHUNK_SORT;
}
