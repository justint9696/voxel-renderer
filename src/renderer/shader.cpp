#include "renderer/shader.hpp"
#include "logger.hpp"

#include <glm/glm.hpp>

#include <cstring>

Shader::Shader(std::string_view frag_path, std::string_view vert_path) {
    uint32_t vert_shader = this->create(GL_VERTEX_SHADER, vert_path);
    uint32_t frag_shader = this->create(GL_FRAGMENT_SHADER, frag_path);

    this->handle = glCreateProgram();
    glAttachShader(this->handle, vert_shader);
    glAttachShader(this->handle, frag_shader);
    glLinkProgram(this->handle);

    int32_t success;
    glGetProgramiv(this->handle, GL_LINK_STATUS, &success);
    if (!success) {
        char error_buf[512];
        glGetShaderInfoLog(this->handle, sizeof(error_buf), nullptr, error_buf);
        lg::fatal("Failed to link shaders: \n{}", error_buf);
    }

    glDeleteShader(frag_shader);
    glDeleteShader(vert_shader);
}

uint32_t Shader::create(GLenum type, std::string_view fname) {
    FILE *fp;
    if (!(fp = fopen(fname.data(), "rb"))) {
        lg::fatal("Failed to open {}", fname);
    }

    size_t size;
    (void)fseek(fp, 0, SEEK_END);

    if ((size = ftell(fp)) == 0) {
        lg::fatal("Shader is empty: {}", fname);
    }

    (void)fseek(fp, 0, SEEK_SET);

    char *text;
    ASSERT((text = new char[size]));

    if (!fread(text, size, 1, fp)) {
        lg::fatal("Failed to read file: {}: {}", fname, strerror(errno));
    }

    ASSERT(strnlen(text, sizeof(text)) > 0);

    uint32_t shader = glCreateShader(type);
    glShaderSource(shader, 1, (const GLchar *const *)&text, (const GLint *)&size);
    glCompileShader(shader);

    int32_t success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char error_buf[1024];
        glGetShaderInfoLog(shader, sizeof(error_buf), nullptr, error_buf);
        lg::fatal("Failed to compile shader: \n{}", error_buf);
    }

    ASSERT(fp);
    ASSERT(!fclose(fp));
    ASSERT(text);

    delete[] text;
    return shader;
}

template <>
void Shader::set(std::string_view name, glm::vec3 value) {
    glUniform3fv(glGetUniformLocation(this->handle, name.data()), 1,
                 (const GLfloat *)&value);
}

template <>
void Shader::set(std::string_view name, glm::vec4 value) {
    glUniform4fv(glGetUniformLocation(this->handle, name.data()), 1,
                 (const GLfloat *)&value);
}

template <>
void Shader::set(std::string_view name, float value) {
    glUniform1f(glGetUniformLocation(this->handle, name.data()), value);
}

template <>
void Shader::set(std::string_view name, glm::mat4 value) {
    glUniformMatrix4fv(glGetUniformLocation(this->handle, name.data()),
                       1, GL_FALSE, (const GLfloat *)&value);
}
