#pragma once

#include "game/camera.hpp"
#include "renderer/shader.hpp"
#include "renderer/texture.hpp"
#include "world/chunk.hpp"

class ChunkRenderer {
public:
    explicit ChunkRenderer(
            Chunk& chunk,
            Shader& shader,
            Texture& texture) 
        : chunk(chunk), shader(shader), texture(texture) {}

    ~ChunkRenderer() = default;

    void prepare();
    void render(const Camera& camera);

private:
    void remesh(ChunkSection& section);

private:
    Chunk& chunk;
    Shader& shader;
    Texture& texture;
    size_t cmd = 0;
};
