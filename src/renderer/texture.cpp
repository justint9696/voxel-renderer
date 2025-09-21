#include "renderer/texture.hpp"
#include "logger.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

Texture::Texture(const std::string& fname) {
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

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
                 GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    this->size = { width, height };
    lg::debug("Created texture {} of size ({}, {})", fname, width, height);

    assert(data);
    stbi_image_free(data);
}
