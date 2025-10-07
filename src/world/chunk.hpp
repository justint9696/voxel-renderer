#pragma once

#include "game/camera.hpp"
#include "world/section.hpp"

#include <glm/vec3.hpp>
#include <vector>

class Chunk {
public:
    Chunk() = default;
    ~Chunk() = default;

    Chunk(glm::vec3 position, uint32_t view_distance = 1);

    // Draws all the chunk sections
    void render(const Camera& cam);

private:
    std::vector<ChunkSection> sections;
    glm::vec3 position;

    uint32_t view_distance;
};
