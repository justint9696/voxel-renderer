#include "world/world.hpp"

#include <glm/glm.hpp>

World::World(glm::vec3 pos, glm::ivec2 viewport)
        : chunk(pos, 8) {
    this->camera = PerspectiveCamera(this->chunk.center(), 90.0f, viewport);
    this->nchunks = std::pow((this->chunk.view_distance * 2) + 1, 2);
}

void World::tick(float dt) {
    if (!this->chunk.position_in_center(this->camera.position)) {
        this->chunk.swap(this->camera.position);
    }
}
