#include "world/world.hpp"

#include <glm/glm.hpp>

constexpr glm::ivec2 SCREEN_SIZE = { 1280, 720 };

World::World(glm::vec3 pos) :
        chunk(pos) {
    this->camera = PerspectiveCamera(this->chunk.center(), 90.0f, SCREEN_SIZE);
}

void World::update(float dt) {
    this->chunk.update(dt);
}

void World::tick(float dt) {
    this->camera.tick(dt);
    this->chunk.tick(dt);
    if (!this->chunk.position_in_center(this->camera.position)) {
        this->chunk.swap(this->camera.position);
    }
}

void World::render(void) {
    this->chunk.render(this->camera);
}
