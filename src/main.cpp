#include "renderer/shader.hpp"
#include "renderer/renderer.hpp"
#include "game/camera.hpp"
#include "game/input.hpp"
#include "time.hpp"
#include "logger.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

static constexpr size_t BLOCK_FACES = 6;

static float BLOCK_VERTICES[] = {
    -0.5f, -0.5f, +0.5f, // front
    -0.5f, +0.5f, +0.5f,
    +0.5f, +0.5f, +0.5f,
    +0.5f, -0.5f, +0.5f,

    -0.5f, -0.5f, -0.5f, // back
    -0.5f, +0.5f, -0.5f,
    +0.5f, +0.5f, -0.5f,
    +0.5f, -0.5f, -0.5f,

    -0.5f, +0.5f, -0.5f, // top
    -0.5f, +0.5f, +0.5f,
    +0.5f, +0.5f, +0.5f,
    +0.5f, +0.5f, -0.5f,

    -0.5f, -0.5f, -0.5f, // bottom
    -0.5f, -0.5f, +0.5f,
    +0.5f, -0.5f, +0.5f,
    +0.5f, -0.5f, -0.5f,

    -0.5f, -0.5f, -0.5f, // left
    -0.5f, +0.5f, -0.5f,
    -0.5f, +0.5f, +0.5f,
    -0.5f, -0.5f, +0.5f,

    +0.5f, -0.5f, -0.5f, // right
    +0.5f, +0.5f, -0.5f,
    +0.5f, +0.5f, +0.5f,
    +0.5f, -0.5f, +0.5f,
};

static float BLOCK_UVS[] = {
    0.0f, 0.0f,
    0.0f, 1.0f,
    1.0f, 1.0f,
    1.0f, 0.0f,
};

static float BLOCK_NORMALS[] = {
    0.0f, 0.0f, +1.0f, // front
    0.0f, 0.0f, -1.0f, // back
    0.0f, +1.0f, 0.0f, // top
    0.0f, -1.0f, 0.0f, // bottom
    -1.0f, 0.0f, 0.0f, // left
    +1.0f, 0.0f, 0.0f, // right
};

static uint32_t BLOCK_INDICES[] = {
    0, 1, 3,
    1, 2, 3
};

struct BlockMesh {
    std::vector<float> vertices;
    std::vector<float> uvs;
    std::vector<float> normals;
    std::vector<uint32_t> indices;
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
    uint32_t ibo;
    PerspectiveCamera cam;
    BlockMesh mesh;
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

    for (size_t i = 0; i < BLOCK_FACES; i++) {
        for (size_t j = 0; j < 12; j++) {
            size_t idx = (i * 12) + j;
            g_state.mesh.vertices.push_back(BLOCK_VERTICES[idx]);
        }
    }

    for (size_t i = 0; i < BLOCK_FACES; i++) {
        for (size_t j = 0; j < 8; j++) {
            float n = BLOCK_UVS[j];
            if (n > 0.0f) {
                n = ((j % 2 == 0) ? uv.w : uv.z);
            } else {
                n = ((j % 2 == 0) ? uv.y : uv.x);
            }
            g_state.mesh.uvs.push_back(n);
        }
    }

    for (size_t i = 0; i < BLOCK_FACES; i++) {
        for (size_t j = 0; j < 6; j++) {
            g_state.mesh.indices.push_back(BLOCK_INDICES[j] + (i * 4));
        }
    }

    for (size_t i = 0; i < BLOCK_FACES; i++) {
        for (size_t j = 0; j < 4; j++) {
            for (size_t k = 0; k < 3; k++) {
                g_state.mesh.normals.push_back(BLOCK_NORMALS[k + (i * 3)]);
            }
        }
    }

    lg::debug("Creating mesh from {} vertices, {} uvs, {} normals, and {} indices",
              g_state.mesh.vertices.size(), 
              g_state.mesh.uvs.size(),
              g_state.mesh.normals.size(),
              g_state.mesh.indices.size());

    glGenVertexArrays(1, &g_state.vao);
    glBindVertexArray(g_state.vao);

    glGenBuffers(1, &g_state.vbo);
    glBindBuffer(GL_ARRAY_BUFFER, g_state.vbo);

    size_t size = (g_state.mesh.vertices.size() * sizeof(float));
    size += (g_state.mesh.uvs.size() * sizeof(float));
    size += (g_state.mesh.normals.size() * sizeof(float));

    glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_STATIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER,
                    0,
                    g_state.mesh.vertices.size() * sizeof(float),
                    g_state.mesh.vertices.data());
    glBufferSubData(GL_ARRAY_BUFFER,
                    g_state.mesh.vertices.size() * sizeof(float),
                    g_state.mesh.uvs.size() * sizeof(float),
                    g_state.mesh.uvs.data());
    glBufferSubData(GL_ARRAY_BUFFER,
                    (g_state.mesh.uvs.size() 
                     + g_state.mesh.vertices.size()) * sizeof(float),
                    g_state.mesh.normals.size() * sizeof(float),
                    g_state.mesh.normals.data());

    glGenBuffers(1, &g_state.ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, g_state.ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                 g_state.mesh.indices.size() * sizeof(uint32_t),
                 g_state.mesh.indices.data(),
                 GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float),
                          (void *)(sizeof(float) * g_state.mesh.vertices.size()));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float),
                          (void *)(sizeof(float) * 
                              (g_state.mesh.vertices.size() +
                               g_state.mesh.uvs.size())));
    glEnableVertexAttribArray(2);

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

        glDrawElements(GL_TRIANGLES, g_state.mesh.indices.size(), GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window.handle);
        glfwPollEvents();

        glClearColor(0.52f, 0.81f, 0.92f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    renderer::destroy();
    return 0;
}
