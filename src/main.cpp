#include "logger.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#define ASSERT(_e, _fmt, ...) do { \
    if (!(_e)) { \
        lg::fatal("Assertion failure: {}:{}: " _fmt, \
                  __FUNCTION__, __LINE__, ##__VA_ARGS__); \
    } \
} while (0)

static const char *vshader_src = 
    "#version 330 core\n"
    "layout (location = 0) in vec3 pos;\n"
    "void main() {\n"
    "\rgl_Position = vec4(pos, 1.0f);\n"
    "}\n";

static const char *fshader_src =
    "#version 330 core\n"
    "out vec4 frag_color;\n"
    "void main() {\n"
    "\rfrag_color = vec4(1.0f, 0.65, 0.0f, 1.0f);\n"
    "}";

static float vertices[] = {
    -0.5f, -0.5f, 0.0f,
    +0.5f, -0.5f, 0.0f,
    +0.0f, +0.5f, 0.0f,
};

struct Window {
    glm::ivec2 size{ 640, 480 };
    GLFWwindow *handle;
};

struct State {
    Window window;
    uint32_t vao;
    uint32_t vbo;
    uint32_t shader;
};

static State g_state;

int main(int argc, char *argv[]) {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    g_state.window.handle = glfwCreateWindow(
            g_state.window.size.x, g_state.window.size.y, "Game", 
            NULL, NULL);

    ASSERT(g_state.window.handle, "Failed to create window");
    glfwMakeContextCurrent(g_state.window.handle);

    ASSERT(gladLoadGLLoader((GLADloadproc)glfwGetProcAddress),
            "Failed to initialize GLAD");

    glViewport(0, 0, g_state.window.size.x, g_state.window.size.y);

    glGenVertexArrays(1, &g_state.vao);
    glBindVertexArray(g_state.vao);

    glGenBuffers(1, &g_state.vbo);
    glBindBuffer(GL_ARRAY_BUFFER, g_state.vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    uint32_t vs = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vs, 1, (const char *const *)&vshader_src, NULL);
    glCompileShader(vs);

    int32_t success;
    glGetShaderiv(vs, GL_COMPILE_STATUS, &success);
    ASSERT(success, "Failed to compile vertex shader");

    uint32_t fs = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fs, 1, (const char *const *)&fshader_src, NULL);
    glCompileShader(fs);

    glGetShaderiv(fs, GL_COMPILE_STATUS, &success);
    ASSERT(success, "Failed to compile fragment shader");

    uint32_t shader = glCreateProgram();
    glAttachShader(shader, vs);
    glAttachShader(shader, fs);
    glLinkProgram(shader);

    glGetProgramiv(shader, GL_LINK_STATUS, &success);
    ASSERT(success, "Failed to link shader");

    glDeleteShader(vs);
    glDeleteShader(fs);

    glUseProgram(shader);

    while (!glfwWindowShouldClose(g_state.window.handle)) {
        if (glfwGetKey(g_state.window.handle, GLFW_KEY_Q) == GLFW_PRESS) {
            glfwSetWindowShouldClose(g_state.window.handle, GLFW_TRUE);
        }

        glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwSwapBuffers(g_state.window.handle);
        glfwPollEvents();

        glClearColor(0.52f, 0.81f, 0.92f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
    }

    glfwDestroyWindow(g_state.window.handle);
    glfwTerminate();

    return 0;
}
