#pragma once

#include "game/camera.hpp"
#include "world/section.hpp"

#include <PerlinNoise.hpp>
#include <glm/vec3.hpp>

#include <queue>
#include <vector>

constexpr uint32_t MESH_PER_FRAME = 1;
constexpr uint32_t MESH_TIME_MAX = 50;

class Chunk {
public:
    float vram = 0.0f;
    uint32_t mesh_idx;
    float avg_mesh = 0.0f;
    time_t mesh_time[MESH_TIME_MAX];
    uint32_t view_distance;

public:
    Chunk() = default;
    ~Chunk() = default;

    Chunk(glm::vec3 position, uint32_t view_distance = 2);

    void tick(float dt);
    void update(float dt);
    void render(const Camera& cam);

    inline size_t queued() const {
        return this->queue.size();
    }

    inline float mesh_avg() {
        return this->avg_mesh;
    }

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

    // Perlin noise
    siv::PerlinNoise perlin;

    // Chunk section queue
    std::deque<ChunkSection *> queue;

    uint32_t mesh_count;

private:
    // Prepares the chunk mesh for rendering
    void prepare_mesh(ChunkSection& section);

    // Determines the chunk section that contains a given position
    ChunkSection *section_from_position(glm::vec3 position);

    // Generates a chunk section
    void generate(ChunkSection& section, glm::vec3 position);

    // Regenerates a chunk section
    void regenerate(ChunkSection& section);

    // Determines the chunk parameters at a given position
    void params_at(ChunkParams& params, float x, float z);
};
