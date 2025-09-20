#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>

class Shader {
public:
    Shader() = default;
    ~Shader() = default;

    Shader(const std::string& frag_path, const std::string& vert_path);

    inline void use() const {
        if (this->handle) {
            glUseProgram(this->handle);
        }
    }

    template <typename Tp>
    void set(const std::string& name, Tp value);

private:
    uint32_t handle;

private:
    uint32_t create(GLenum type, const std::string& fname);
};
