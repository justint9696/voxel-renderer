#include "world/chunk.hpp"

#include "renderer/renderer.hpp"
#include "logger.hpp"

#include <cmath>

Chunk::Chunk(glm::vec3 position, uint32_t view_distance) :
        position(position), view_distance(view_distance) {
    glm::vec3 pos;
    size_t nchunks = (this->view_distance * 2) + 1;
    lg::info("Creating {} chunk sections", std::pow(nchunks, 2));

    for (float x = 0.0f; x < nchunks; x++) {
        for (float z = 0.0f; z < nchunks; z++) {
            pos = this->position +
                  glm::vec3(x * CHUNK_WIDTH, 0.0f, z * CHUNK_DEPTH);
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
    shader.set<glm::vec4>("u_color", glm::vec4(1.0f));;

    for (auto& section : this->sections) {
        section.render();
    }
}
