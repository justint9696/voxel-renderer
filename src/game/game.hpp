#pragma once

#include "renderer/chunk_renderer.hpp"
#include "renderer/shader.hpp"
#include "renderer/texture.hpp"
#include "renderer/window.hpp"
#include "world/world.hpp"
#include "player.hpp"

#include <memory>
#include <unordered_map>

struct Time {
    Time(uint64_t ts) 
        : now(ts)
        , last_second(ts)
        , last_tick(ts)
        , last_frame(ts) {}

    uint64_t now = 0;
    uint64_t last_second = 0;
    uint64_t last_tick = 0;
    uint64_t last_frame = 0;
    uint32_t fps = 0;
    uint32_t tps = 0;
};

class Game {
public:
    Game(std::string_view title, glm::ivec2 size);

    void run();

private:
    void tick(float dt);
    void update(float dt);
    void render();

private:
    Time time;
    World world;
    Player player;
    Window window;
    std::unique_ptr<ChunkRenderer> renderer;
    std::unordered_map<std::string, Shader> shader_mgr;
    std::unordered_map<std::string, Texture> texture_mgr;
};
