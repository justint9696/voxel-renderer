#pragma once

#include "game/camera.hpp"
#include "world/chunk.hpp"

class World {
public:
    size_t nchunks;
    Chunk chunk;
    PerspectiveCamera camera;

public:
    World() = default;
    ~World() = default;

    World(glm::vec3 pos, glm::ivec2 viewport);

    void tick(float dt);
};
