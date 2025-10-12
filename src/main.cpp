#include "game/input.hpp"
#include "renderer/renderer.hpp"
#include "world/world.hpp"
#include "logger.hpp"
#include "time.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

constexpr glm::ivec2 SCREEN_SIZE = { 1280, 720 };

struct State {
    bool wireframe = false;
    time_t now;
    time_t last_second = 0;
    time_t last_tick = 0;
    time_t last_frame = 0;
    uint32_t fps = 0;
    uint32_t tps = 0;
    World world;
};

static State g_state;

static void init(void);
static void update(float dt);
static void tick(float dt);
static void render(void);
static void destroy(void);

int main(int argc, char *argv[]) {
    init();
    const Window& window = renderer::get_window();

    float dt, fixed_dt;
    fixed_dt = (1.0f * NS_PER_TICK / NS_PER_SECOND);

    while (true) {
        glfwPollEvents();
        glfwSwapBuffers(window.handle);

        glClearColor(0.52f, 0.81f, 0.92f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        if (input::key_pressed(GLFW_KEY_Q)) {
            break;
        }

        if (input::key_pressed(GLFW_KEY_T)) {
            g_state.wireframe ^= 1;
            glPolygonMode(GL_FRONT_AND_BACK,
                          ((g_state.wireframe) ? GL_LINE : GL_FILL));
        }

        g_state.last_frame = g_state.now;
        g_state.now = util::time::now();
        dt = (1.0f * (g_state.now - g_state.last_frame) / NS_PER_SECOND);

        if ((g_state.now - g_state.last_second) > NS_PER_SECOND) {
            window.set_title("Game - FPS: {} - TPS: {}",
                             g_state.fps, g_state.tps);
            g_state.last_second = g_state.now;
            g_state.fps = 0;
            g_state.tps = 0;
        }

        while ((g_state.now - g_state.last_tick) > NS_PER_TICK) {
            tick(fixed_dt);
            g_state.last_tick += NS_PER_TICK;
            g_state.tps++;
        }

        g_state.fps++;

        update(dt);
        render();
    }

    destroy();
    return 0;
}

static void init(void) {
    lg::set_file("output.log");
    lg::set_stdout_level(lg::LogLevel::Info);

    renderer::init(SCREEN_SIZE, "Game");

    renderer::shader::create(
            "default",
            "shaders/default.frag.glsl",
            "shaders/default.vert.glsl");
    renderer::shader::use("default");

    renderer::texture::create("atlas", "assets/atlas.png", { 16, 16 });

    g_state.world = World({ 0.0f, 0.0f, 0.0f });

    g_state.now = util::time::now();
    g_state.last_second = g_state.now;
    g_state.last_tick = g_state.now;
}

static void update(float dt) {
    g_state.world.update(dt);
}

static void tick(float dt) {
    g_state.world.tick(dt);
}

static void render(void) {
    g_state.world.render();
}

static void destroy(void) {
    renderer::destroy();
}
