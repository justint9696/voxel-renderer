#pragma once

#include "renderer/shader.hpp"
#include "renderer/window.hpp"

#include <glm/vec4.hpp>
#include <string>

namespace renderer {
    void init(glm::vec2 viewport, const std::string& title);
    void destroy();
    const Window& get_window();

    namespace shader {
        void create(const std::string& name, const std::string& frag_path,
                    const std::string& vert_path);
        void destroy(const std::string& name);
        void use(const std::string& name);
        Shader& get(const std::string& name);
    }

    namespace texture {
        void create(const std::string& name, const std::string& fpath,
                    glm::ivec2 size = { 1, 1 });
        void destroy(const std::string& name);
        void bind(const std::string& name);
        glm::vec4 uv_coords(const std::string& name, uint32_t idx);
    }
}
