#pragma once

#include "game/camera.hpp"
#include "world/chunk.hpp"

class World {
public:
    Chunk chunk;
    PerspectiveCamera camera;

public:
    World() = default;
    ~World() = default;

    World(glm::vec3 pos);

    void update(float dt);
    void tick(float dt);
    void render(void);
};
