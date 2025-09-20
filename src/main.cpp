#include "renderer/shader.hpp"
#include "renderer/renderer.hpp"
#include "logger.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

static float vertices[] = {
    -0.5f, -0.5f, 0.0f,
    +0.5f, -0.5f, 0.0f,
    +0.0f, +0.5f, 0.0f,
};

struct State {
    uint32_t vao;
    uint32_t vbo;
    Shader shader;
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

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    renderer::shader::create(
            "default",
            "shaders/default.frag.glsl",
            "shaders/default.vert.glsl");
    renderer::shader::use("default");

    const Window& window = renderer::get_window();
    while (window.is_open()) {
        if (glfwGetKey(window.handle, GLFW_KEY_Q) == GLFW_PRESS) {
            window.close();
        }

        float a = fabs(sin(glfwGetTime()));
        float b = fabs(1 - sin(glfwGetTime()));
        float c = fabs(cos(glfwGetTime()));
        renderer::shader::get("default").set<glm::vec4>("u_color", glm::vec4(a, b, c, 1.0f));

        glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwSwapBuffers(window.handle);
        glfwPollEvents();

        glClearColor(0.52f, 0.81f, 0.92f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
    }

    renderer::destroy();
    return 0;
}
