#include "renderer/font.hpp"

#include <glm/glm.hpp>

constexpr glm::ivec2 INVAL_CHAR = { -1, -1 };

const std::string_view FONT_TABLE[4] = {
    "ABCDEFGHIJKLMNOPQRSTUVW",
    "XYZabcdefghijklmnopqrst",
    "uvwxyz1234567890-=_+[]{",
    "};:'\",<.>/?\\|()"
};

extern const glm::vec2 FONT_VERTICES[];
extern const glm::vec2 FONT_UVS[];
extern const uint32_t FONT_INDICES[];

Font::Font(const std::string& frag_path, const std::string& vert_path,
           const std::string& tex_path, glm::ivec2 size) :
        shader(frag_path, vert_path),
        texture(tex_path, size) {
    glGenVertexArrays(1, &this->vao);
    glGenBuffers(1, &this->vbo);
    glGenBuffers(1, &this->ibo);
}

static glm::ivec2 get_ch(const char ch) {
    for (size_t i = 0; i < 4; i++) {
        const auto& font = FONT_TABLE[i];
        for (size_t j = 0; j < font.length(); j++) {
            if (font.at(j) == ch) {
                return { j, 3 - i };
            }
        }
    }

    return INVAL_CHAR;
}

void Font::create(const std::string_view& text, glm::vec2 pos, float scale) {
    glm::ivec2 idx;
    for (const auto& ch : text) {
        if ((idx = get_ch(ch)) == INVAL_CHAR) {
            pos.x += scale * 0.5f;
            continue;
        }

        for (size_t i = 0; i < 4; i++) {
            const auto& vertex = FONT_VERTICES[i];
            this->vertices.emplace_back(pos + glm::vec2(
                (vertex.x < 1.0f) ? vertex.x : vertex.x + scale,
                (vertex.y < 1.0f) ? vertex.y : vertex.y + scale
            ));
        }

        auto uvs = this->texture.uv_coords(idx);
        for (size_t i = 0; i < 4; i++) {
            const auto& uv = FONT_UVS[i];
            this->uvs.emplace_back(
                ((uv.x > 0.0f) ? uvs.z : uvs.x),
                ((uv.y > 0.0f) ? uvs.w : uvs.y)
            );
        }

        size_t size = (this->indices.size() / 6);
        for (size_t i = 0; i < 6; i++) {
            this->indices.emplace_back(FONT_INDICES[i] + (size * 4));
        }

        pos.x += scale;
    }
}

void Font::render(const Camera& camera) {
    size_t size = 0;
    size_t nbytes = 0;
    off_t offset = 0;

    this->shader.use();
    this->texture.bind();
    
    this->shader.set<glm::mat4>("u_projection", camera.projection);

    const float scale = 2.0f;
    const auto& viewport = camera.get_viewport();
    auto model = glm::translate(glm::mat4(1.0f),
                                glm::vec3(0.0f, viewport.y, 0.0f));
    model = glm::scale(model, glm::vec3(scale, scale, 0.0f));
    this->shader.set<glm::mat4>("u_model", model);


    glBindVertexArray(this->vao);
    glBindBuffer(GL_ARRAY_BUFFER, this->vbo);

    nbytes += sizeof(glm::vec2) * this->vertices.size();
    nbytes += sizeof(glm::vec2) * this->uvs.size();
    glBufferData(GL_ARRAY_BUFFER, nbytes, nullptr, GL_STREAM_DRAW);

    size = this->vertices.size() * sizeof(glm::vec2);
    glBufferSubData(GL_ARRAY_BUFFER, offset, size, this->vertices.data());
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2),
                          (void *)offset);
    glEnableVertexAttribArray(0);

    offset += size;
    size = this->uvs.size() * sizeof(glm::vec2);
    glBufferSubData(GL_ARRAY_BUFFER, offset, size, this->uvs.data());
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2),
                          (void *)offset);
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                 this->indices.size() * sizeof(uint32_t),
                 this->indices.data(),
                 GL_STREAM_DRAW);

    glDrawElements(GL_TRIANGLES, this->indices.size(), GL_UNSIGNED_INT, 0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    this->clear();
}

void Font::clear(void) {
    this->vertices.clear();
    this->uvs.clear();
    this->indices.clear();
}

const glm::vec2 FONT_VERTICES[] = {
    { 0.0f, 0.0f },
    { 0.0f, 1.0f },
    { 1.0f, 1.0f },
    { 1.0f, 0.0f }
};

const glm::vec2 FONT_UVS[] = {
    { 0.0f, 0.0f },
    { 0.0f, 1.0f },
    { 1.0f, 1.0f },
    { 1.0f, 0.0f }
};

const uint32_t FONT_INDICES[] = {
    0, 3, 1, 3, 2, 1
};
