#include "renderer/shader.hpp"
#include "renderer/renderer.hpp"
#include "time.hpp"
#include "logger.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <cstring>

static float vertices[] = {
    -0.5f, -0.5f, 0.0f,
    +0.5f, -0.5f, 0.0f,
    +0.0f, +0.5f, 0.0f,
};

struct State {
    time_t now;
    time_t last_second;
    time_t last_tick;
    uint32_t fps;
    uint32_t tps;
    uint32_t vao;
    uint32_t vbo;
};

static State g_state;

int main(int argc, char *argv[]) {
    lg::set_file("output.log");

    memset(&g_state, 0, sizeof(g_state));
    renderer::init(glm::vec2(640, 480), "Game");

    glGenVertexArrays(1, &g_state.vao);
    glBindVertexArray(g_state.vao);

    glGenBuffers(1, &g_state.vbo);
    glBindBuffer(GL_ARRAY_BUFFER, g_state.vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    renderer::shader::create(
            "default",
            "shaders/default.frag.glsl",
            "shaders/default.vert.glsl");
    renderer::shader::use("default");

    const Window& window = renderer::get_window();
    Shader& shader = renderer::shader::get("default");

    g_state.now = util::time::now();
    g_state.last_second = g_state.now;
    g_state.last_tick = g_state.now;
    g_state.last_tick = g_state.now;

    while (window.is_open()) {
        if (glfwGetKey(window.handle, GLFW_KEY_Q) == GLFW_PRESS) {
            window.close();
            break;
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
