#pragma once

#include "game/camera.hpp"
#include "world/section.hpp"

#include <glm/vec3.hpp>
#include <vector>

constexpr uint32_t MESH_PER_FRAME = 1;

class Chunk {
public:
    Chunk() = default;
    ~Chunk() = default;

    Chunk(glm::vec3 position, uint32_t view_distance = 2);

    void tick(float dt);
    void update(float dt);
    void render(const Camera& cam);

    // Returns the center of the chunk
    glm::vec3 center(void) const;

    // Swaps the current chunks
    void swap(glm::vec3 position);

    // Checks if a position position is within the center chunk
    bool position_in_center(glm::vec3 position);

private:
    std::vector<ChunkSection> sections;

    // The bottom-left corner coordinates of the chunk
    glm::vec3 position;

    uint32_t mesh_count;
    uint32_t view_distance;

private:
    void prepare_mesh(ChunkSection& section);
    ChunkSection *section_from_position(glm::vec3 position);
};
