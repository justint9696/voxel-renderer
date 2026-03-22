#include "game/game.hpp"
#include "time.hpp"

Game::Game(std::string_view title, glm::ivec2 size) 
        : time(util::time::now())
        , window(size, title) {
    this->shader_mgr.try_emplace(
            "default",
            "shaders/default.frag.glsl",
            "shaders/default.vert.glsl");
    this->shader_mgr.try_emplace(
            "font",
            "shaders/font.frag.glsl",
            "shaders/font.vert.glsl");
    this->texture_mgr.try_emplace(
            "atlas",
            "assets/atlas.png",
            glm::ivec2 { 16, 16 });
    this->texture_mgr.try_emplace(
            "font",
            "assets/font.png",
            glm::ivec2 { 11, 17 });
    this->renderer = std::make_unique<ChunkRenderer>(
            this->world.chunk,
            this->shader_mgr.at("default"),
            this->texture_mgr.at("atlas"));
    this->world = World(glm::vec3(0.0f), size);
}

void Game::run() {
    static constexpr float fixed_dt = (1.0f * NS_PER_TICK / NS_PER_SECOND);

    this->window.clear_color(0.52f, 0.81f, 0.92f, 1.0f);

    while (true) {
        if (this->window.should_close())
            break;

        this->window.clear();

        this->time.now = util::time::now();
        auto dt =
            (1.0f - this->time.now - this->time.last_frame) / NS_PER_SECOND;

        if ((this->time.now - this->time.last_second) > NS_PER_SECOND) {
            window.set_title("Game - FPS: {} - TPS: {}",
                            this->time.fps, this->time.tps);
            this->time.last_second = this->time.now;
            this->time.fps = 0;
            this->time.tps = 0;
        }

        while ((this->time.now - this->time.last_tick) > NS_PER_TICK) {
            this->time.last_tick += NS_PER_TICK;
            this->time.tps++;
            this->tick(fixed_dt);
        }

        this->update(dt);
        this->render();

        this->time.fps++;

        this->window.poll();
        this->window.swap();
    }
}

void Game::update(float dt) {}

void Game::tick(float dt) {
    if (this->window.input.key_pressed(GLFW_KEY_Q)) {
        this->window.close();
    }
    if (this->window.input.key_pressed(GLFW_KEY_F12)) {
        this->flags ^= GAME_DEBUG;
    } 

    if (this->window.input.key_pressed(GLFW_KEY_T)) {
        this->flags ^= GAME_WIREFRAME;
        glPolygonMode(GL_FRONT_AND_BACK,
                ((this->flags & GAME_WIREFRAME) ? GL_LINE : GL_FILL));
    }

    this->world.tick(dt);
    this->player.tick(this->window.input, this->world.camera, dt);
    this->window.input.mouse_reset();
    this->window.input.tick();

    if (auto renderer = this->renderer.get()) {
        renderer->prepare();
    }
}

void Game::render() {
    if (auto renderer = this->renderer.get()) {
        renderer->render(this->world.camera);
    }
}
