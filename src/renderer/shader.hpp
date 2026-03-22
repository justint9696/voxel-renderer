#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string_view>

class Shader {
public:
    Shader() = default;
    ~Shader() = default;

    Shader(std::string_view frag_path, std::string_view vert_path);

    inline void use() const {
        if (this->handle) {
            glUseProgram(this->handle);
        }
    }

    // Sets a shader uniform by type
    template <typename Tp>
    void set(std::string_view name, Tp value);

private:
    uint32_t handle = 0;

private:
    uint32_t create(GLenum type, std::string_view fname);
};
