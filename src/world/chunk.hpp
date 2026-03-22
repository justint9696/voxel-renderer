#pragma once

#include "game/camera.hpp"
#include "world/section.hpp"

#include <PerlinNoise.hpp>
#include <glm/vec3.hpp>

#include <deque>
#include <vector>

constexpr uint32_t MESH_PER_FRAME = 2;
constexpr uint32_t MESH_TIME_MAX = 50;

constexpr uint32_t CHUNK_SORT = (1ULL << 0);

class Chunk {
public:
    float vram = 0.0f;
    uint32_t mesh_idx;
    float avg_mesh = 0.0f;
    time_t mesh_time[MESH_TIME_MAX];
    uint32_t view_distance;
    uint32_t flags = 0;
    size_t nvertices = 0;
    uint32_t mesh_count;

    std::vector<ChunkSection> sections;
    std::vector<uint32_t> indices;

    // Chunk section queue
    std::deque<uint32_t> queue;

public:
    Chunk() = default;
    ~Chunk() = default;

    Chunk(glm::vec3 position, uint32_t view_distance = 5);

    inline size_t queued() const {
        return this->queue.size();
    }

    inline float mesh_avg() const {
        return this->avg_mesh;
    }

    // Returns the center of the chunk
    glm::vec3 center(void) const;

    // Swaps the current chunks
    void swap(glm::vec3 position);

    // Checks if a position position is within the center chunk
    bool position_in_center(glm::vec3 position);

    // Sorts the chunks and indices based on distance from the camera
    void sort(const Camera& camera);

    // Determines the chunk section that contains a given position
    ChunkSection *section_from_position(glm::vec3 position);

    // Generates a chunk section
    void generate(ChunkSection& section, glm::vec3 position);

    // Regenerates a chunk section
    void regenerate(ChunkSection& section);

    // Determines the chunk parameters at a given position
    void params_at(ChunkParams& params, float x, float z);

private:

    // The bottom-left corner coordinates of the chunk
    glm::vec3 position;

    // Perlin noise
    siv::PerlinNoise perlin;
};
