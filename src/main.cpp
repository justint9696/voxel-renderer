#include "renderer/chunk_mesh.hpp"
#include "renderer/shader.hpp"
#include "renderer/renderer.hpp"
#include "game/camera.hpp"
#include "game/input.hpp"
#include "world/block.hpp"
#include "time.hpp"
#include "logger.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

struct State {
    bool wireframe = false;
    time_t now;
    time_t last_second = 0;
    time_t last_tick = 0;
    uint32_t fps = 0;
    uint32_t tps = 0;
    PerspectiveCamera cam;
    ChunkMesh mesh;
};

static State g_state;

int main(int argc, char *argv[]) {
    lg::set_file("output.log");

    renderer::init(glm::vec2(640, 480), "Game");

    g_state.cam =
        PerspectiveCamera(glm::vec3(0.0f), 45.0f, { 640, 480 });

    renderer::shader::create(
            "default",
            "shaders/default.frag.glsl",
            "shaders/default.vert.glsl");
    renderer::shader::use("default");

    renderer::texture::create("default", "assets/atlas.png", { 16, 16 });
    renderer::texture::bind("default");
    auto uv = renderer::texture::uv_coords("default", 0);

    const Window& window = renderer::get_window();
    Shader& shader = renderer::shader::get("default");

    shader.set<glm::mat4>("u_view", g_state.cam.view);
    shader.set<glm::mat4>("u_projection", g_state.cam.projection);

    g_state.mesh.init();
    for (size_t i = 0; i < BLOCK_FACES; i++) {
        for (size_t j = 0; j < 4; j++) {
            g_state.mesh.vertices.push_back(BLOCK_VERTICES[j + (i * 4)]);
        }
    }

    for (size_t i = 0; i < BLOCK_FACES; i++) {
        for (size_t j = 0; j < 4; j++) {
            glm::vec2 n = BLOCK_UVS[j];
            g_state.mesh.uvs.push_back({
                ((n.x < 1.0f) ? uv.x : uv.z),
                ((n.y < 1.0f) ? uv.y : uv.w)
            });
        }
    }

    for (size_t i = 0; i < BLOCK_FACES; i++) {
        for (size_t j = 0; j < 4; j++) {
            g_state.mesh.normals.push_back(BLOCK_NORMALS[i]);
        }
    }

    for (size_t i = 0; i < BLOCK_FACES; i++) {
        for (size_t j = 0; j < 6; j++) {
            g_state.mesh.indices.push_back(BLOCK_INDICES[j] + (i * 4));
        }
    }

    g_state.mesh.allocate(false);
    g_state.mesh.submit();

    g_state.now = util::time::now();
    g_state.last_second = g_state.now;
    g_state.last_tick = g_state.now;

    while (window.is_open()) {
        if (input::key_pressed(GLFW_KEY_Q)) {
            window.close();
            break;
        }

        if (input::key_pressed(GLFW_KEY_T)) {
            g_state.wireframe ^= 1;
            glPolygonMode(GL_FRONT_AND_BACK,
                          ((g_state.wireframe) ? GL_LINE : GL_FILL));
        }

        g_state.now = util::time::now();
        if ((g_state.now - g_state.last_second) > NS_PER_SECOND) {
            window.set_title("Game - FPS: {} - TPS: {}",
                             g_state.fps, g_state.tps);
            g_state.last_second = g_state.now;
            g_state.fps = 0;
            g_state.tps = 0;
        }

        while ((g_state.now - g_state.last_tick) > NS_PER_TICK) {
            g_state.last_tick += NS_PER_TICK;
            g_state.tps++;
        }

        g_state.fps++;

        float a = fabs(sin(glfwGetTime()));
        float b = fabs(1 - sin(glfwGetTime()));
        float c = fabs(cos(glfwGetTime()));

        shader.set<glm::vec4>("u_color", glm::vec4(a, b, c, 1.0f));

        auto model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -5.0f));
        model = glm::rotate(model,
                    static_cast<float>(glfwGetTime() * glm::radians(45.0f)),
                    glm::vec3(0.5f, 0.5f, 0.0f));
        shader.set<glm::mat4>("u_model", model);

        g_state.mesh.render();

        glfwSwapBuffers(window.handle);
        glfwPollEvents();

        glClearColor(0.52f, 0.81f, 0.92f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    renderer::destroy();
    return 0;
}
