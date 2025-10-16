#pragma once

#include "game/camera.hpp"
#include "renderer/shader.hpp"
#include "renderer/texture.hpp"

#include <glm/vec2.hpp>

class Font {
public:
    Font() = default;
    ~Font() = default;

    Font(const std::string& frag_path, const std::string& vert_path,
         const std::string& tex_path, glm::ivec2 size);

    void create(const std::string_view& text, glm::vec2 pos, float scale);
    void render(const Camera& camera);
    void clear(void);

private:
    uint32_t vao;
    uint32_t vbo;
    uint32_t ibo;

    std::vector<uint32_t> indices;
    std::vector<glm::vec2> vertices;
    std::vector<glm::vec2> uvs;
    Shader shader;
    Texture texture;
};
