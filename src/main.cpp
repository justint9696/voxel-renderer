#include "renderer/shader.hpp"
#include "renderer/renderer.hpp"
#include "game/input.hpp"
#include "time.hpp"
#include "logger.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

static float vertices[] = {
    -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
    +0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
    +0.0f, +0.5f, 0.0f, 0.5f, 1.0f,
};

struct State {
    bool wireframe = false;
    time_t now;
    time_t last_second = 0;
    time_t last_tick = 0;
    uint32_t fps = 0;
    uint32_t tps = 0;
    uint32_t vao;
    uint32_t vbo;
};

static State g_state;

int main(int argc, char *argv[]) {
    lg::set_file("output.log");

    renderer::init(glm::vec2(640, 480), "Game");

    glGenVertexArrays(1, &g_state.vao);
    glBindVertexArray(g_state.vao);

    glGenBuffers(1, &g_state.vbo);
    glBindBuffer(GL_ARRAY_BUFFER, g_state.vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
                          (void *)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
                          (void *)(sizeof(float) * 3));
    glEnableVertexAttribArray(1);

    renderer::shader::create(
            "default",
            "shaders/default.frag.glsl",
            "shaders/default.vert.glsl");
    renderer::shader::use("default");

    renderer::texture::create("default", "assets/brick.jpg");
    renderer::texture::bind("default");

    const Window& window = renderer::get_window();
    Shader& shader = renderer::shader::get("default");

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

        glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwSwapBuffers(window.handle);
        glfwPollEvents();

        glClearColor(0.52f, 0.81f, 0.92f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
    }

    renderer::destroy();
    return 0;
}
