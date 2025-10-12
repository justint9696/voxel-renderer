#include "world/chunk.hpp"

#include "renderer/renderer.hpp"
#include "logger.hpp"

#include <cmath>

Chunk::Chunk(glm::vec3 position, uint32_t view_distance) :
        position(position), view_distance(view_distance) {
    size_t nchunks = (view_distance * 2) + 1;
    lg::info("Creating {} chunk sections", std::pow(nchunks, 2));

    glm::vec3 pos;
    for (int32_t x = 0; x < nchunks; x++) {
        for (int32_t z = 0; z < nchunks; z++) {
            pos = this->position +
                  glm::vec3(x * CHUNK_WIDTH, 0.0f, z * CHUNK_DEPTH);
            this->sections.emplace_back(pos);
        }
    }

    lg::info("Chunk spawned at ({}, {}, {})",
             this->position.x, this->position.y, this->position.z);
}

void Chunk::update(float dt) {
    this->mesh_count = 0;
}

void Chunk::render(const Camera& cam) {
    auto& shader = renderer::shader::get("default");
    shader.use();

    renderer::texture::bind("atlas");

    shader.set<glm::mat4>("u_view", cam.view);
    shader.set<glm::mat4>("u_projection", cam.projection);
    shader.set<glm::vec4>("u_color", glm::vec4(1.0f));

    for (auto& section : this->sections) {
        if (section.is_dirty() && this->mesh_count < this->mesh_per_frame) {
            this->mesh_count++;
            section.prepare_mesh();
        }

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

bool Chunk::in_center(glm::vec3 position) {
    auto center =
        this->position + (glm::vec3(CHUNK_WIDTH, 0.0f, CHUNK_DEPTH) *
                          static_cast<float>(this->view_distance));
    for (auto& section : this->sections) {
        if (section.get_position() == center)
            return section.contains(position);
    }

    return false;
}

void Chunk::swap(glm::vec3 position) {
    if (this->mesh_count++ >= this->mesh_per_frame)
        return;

    auto norm = glm::vec3(0.0f);

    // determine the swap direction norm
    auto center =
        this->position + (glm::vec3(CHUNK_WIDTH, 0.0f, CHUNK_DEPTH) *
                          static_cast<float>(this->view_distance));
    if (position.x < center.x)
        norm.x = -1.0f;
    else if (position.x >= center.x + CHUNK_WIDTH)
        norm.x = 1.0f;
    else if (position.z < center.z)
        norm.z = -1.0f;
    else if (position.z >= center.z + CHUNK_DEPTH)
        norm.z = 1.0f;

    // update the chunk position by direction norm
    auto vdir = norm * glm::vec3(CHUNK_WIDTH, 0.0f, CHUNK_DEPTH);
    this->position += vdir;

    // regen sections that are outside bounds
    float dist = static_cast<float>((this->view_distance * 2) + 1);
    for (auto& section : this->sections) {
        auto pos = section.get_position();
        if ((pos.x < this->position.x ||
             pos.x >= this->position.x + (CHUNK_WIDTH * dist)) ||
            (pos.z < this->position.z ||
             pos.z >= this->position.z + (CHUNK_DEPTH * dist))) {
            section.generate(pos + (vdir * dist));
        }
    }
}
