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
            lg::info("Creating chunk at ({}, {}, {})",
                      pos.x, pos.y, pos.z);
            this->sections.emplace_back(pos);
        }
    }
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
