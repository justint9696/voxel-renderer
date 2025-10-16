#pragma once

#include "renderer/font.hpp"
#include "renderer/shader.hpp"
#include "renderer/window.hpp"

#include <glm/vec4.hpp>
#include <format>
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

    namespace font {
        namespace intern {
            void create_font_str(const std::string& name, glm::vec2 pos,
                                 float scale, const std::string& text);
        }

        void create(const std::string& name, const std::string& frag_path,
                    const std::string& vert_path, const std::string tex_path,
                    glm::ivec2 size = { 1, 1 });

        Font& get(const std::string& name);

        template <typename... Args>
        void draw(const std::string& name, glm::vec2 pos, float scale,
                  const std::string& fmt, Args&&... args) {
            auto text = std::vformat(fmt, std::make_format_args(args...));
            intern::create_font_str(name, pos, scale, text);
        }
    }
}
