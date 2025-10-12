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

    void update(float dt);
    void render(const Camera& cam);

    // Returns the center of the chunk
    glm::vec3 center(void) const;

    // Swaps the current chunks
    void swap(glm::vec3 position);

    // Checks if a position position is within the center chunk
    bool in_center(glm::vec3 position);

private:
    std::vector<ChunkSection> sections;
    glm::vec3 position;

    uint32_t mesh_per_frame = 2;
    uint32_t mesh_count;
    uint32_t view_distance;
};
