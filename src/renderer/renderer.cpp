#include "renderer/renderer.hpp"
#include "renderer/texture.hpp"
#include "logger.hpp"

#include <map>

namespace renderer {
    class Renderer {
        public:
            Window window;
            std::map<std::string, Shader> shaders;
            std::map<std::string, Texture> textures;

        public:
            Renderer() = default;
            ~Renderer() = default;

            Renderer(glm::vec2 viewport, const std::string& title) :
                window(viewport, title) {}
    };

    static Renderer g_renderer;

    void init(glm::vec2 viewport, const std::string& title) {
        glfwInit();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

        g_renderer = Renderer(viewport, title);
        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
            lg::fatal("Failed to initialize GLAD");
        }

        glfwSwapInterval(0);

        glEnable(GL_DEPTH_TEST);
        glViewport(0, 0, viewport.x, viewport.y);
    }

    void destroy() {
        glfwTerminate();
    }

    const Window& get_window() {
        return g_renderer.window;
    }

    namespace shader {
        void create(const std::string& name, const std::string& frag_path,
                    const std::string& vert_path) {
            g_renderer.shaders.insert(
                    std::make_pair(name, Shader(frag_path, vert_path)));
        }

        void destroy(const std::string& name) {
            g_renderer.shaders.erase(name);
        }

        void use(const std::string& name) {
            g_renderer.shaders.at(name).use();
        }

        Shader& get(const std::string& name) {
            return g_renderer.shaders.at(name);
        }
    }

    namespace texture {
        void create(const std::string& name, const std::string& fpath,
                    glm::ivec2 size) {
            g_renderer.textures.insert(
                    std::make_pair(name, Texture(fpath, size)));
        }

        void destroy(const std::string& name) {
            g_renderer.textures.erase(name);
        }

        void bind(const std::string& name) {
            g_renderer.textures.at(name).bind();
        }

        glm::vec4 uv_coords(const std::string& name, uint32_t idx) {
            return g_renderer.textures.at(name).uv_coords(idx);
        }
    }
}
