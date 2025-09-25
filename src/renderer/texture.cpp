#include "renderer/texture.hpp"
#include "logger.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <glm/common.hpp>

Texture::Texture(const std::string& fname, glm::ivec2 size) : size(size) {
    int width, height, channels;
    uint8_t *data = nullptr;

    stbi_set_flip_vertically_on_load(true);
    data = stbi_load(fname.c_str(), &width, &height, &channels, 0); 
    if (!data) {
        lg::fatal("Failed to load image: {}", fname);
    }

    glGenTextures(1, &this->handle);
    glBindTexture(GL_TEXTURE_2D, this->handle);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA,
                 GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    this->dim = { width, height };
    lg::debug("Created texture {} of size ({}, {})", fname, width, height);

    assert(data);
    stbi_image_free(data);
}

glm::vec4 Texture::uv_coords(glm::vec2 pos) {
    assert((pos.x * pos.y) < (this->size.x * this->size.y));
    return {
        (pos.x * this->size.x) / this->dim.x,
        (pos.y * this->size.y) / this->dim.y,
        ((pos.x + 1) * this->size.x) / this->dim.x,
        ((pos.y + 1) * this->size.y) / this->dim.y
    };
}

glm::vec4 Texture::uv_coords(uint32_t idx) {
    return this->uv_coords({
        idx % this->size.x,
        static_cast<int>(glm::floor(idx / this->size.y))
    });
}
