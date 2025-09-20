#include "renderer/shader.hpp"
#include "logger.hpp"

#include <glm/glm.hpp>
#include <fstream>
#include <sstream>

Shader::Shader(const std::string& frag_path, const std::string& vert_path) {
    char info_buf[512];
    int32_t success;
    uint32_t vert_shader = this->create(GL_VERTEX_SHADER, vert_path);
    uint32_t frag_shader = this->create(GL_FRAGMENT_SHADER, frag_path);

    this->handle = glCreateProgram();
    glAttachShader(this->handle, vert_shader);
    glAttachShader(this->handle, frag_shader);
    glLinkProgram(this->handle);

    glGetProgramiv(this->handle, GL_LINK_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(this->handle, 512, NULL, info_buf);
        lg::fatal("Failed to shaders: \n{}", info_buf);
    }

    glDeleteShader(frag_shader);
    glDeleteShader(vert_shader);
}

uint32_t Shader::create(GLenum type, const std::string& fname) {
    std::stringstream sstream;
    std::ifstream fstream(fname);
    std::string src;
    uint32_t shader;
    int32_t success;
    char info_buf[1024];
    const char *cp;

    if (!fstream.is_open()) {
        lg::fatal("Failed to open file: {}", fname);
    }

    sstream << fstream.rdbuf();

    if (sstream.str().empty()) {
        lg::fatal("Shader is empty: {}", fname);
    }

    src = sstream.str();
    cp = src.c_str();

    shader = glCreateShader(type);
    glShaderSource(shader, 1, &cp, NULL);
    glCompileShader(shader);

    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(shader, 512, NULL, info_buf);
        lg::fatal("Failed to compile shader: {}\n{}", fname, info_buf);
    }

    return shader;
}

template <>
void Shader::set(const std::string& name, glm::vec3 value) {
    glUniform3fv(glGetUniformLocation(this->handle, name.c_str()), 1,
                 (const GLfloat *)&value);
}

template <>
void Shader::set(const std::string& name, glm::vec4 value) {
    glUniform4fv(glGetUniformLocation(this->handle, name.c_str()), 1,
                 (const GLfloat *)&value);
}

template <>
void Shader::set(const std::string& name, float value) {
    glUniform1f(glGetUniformLocation(this->handle, name.c_str()), value);
}

template <>
void Shader::set(const std::string& name, glm::mat4 value) {
    glUniformMatrix4fv(glGetUniformLocation(this->handle, name.c_str()),
                       1, GL_FALSE, (const GLfloat *)&value);
}
