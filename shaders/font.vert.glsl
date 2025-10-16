#version 330 core

layout (location = 0) in vec2 a_pos;
layout (location = 1) in vec2 a_uv;

uniform mat4 u_projection;
uniform mat4 u_model;

out vec2 o_uv;

void main() {
    o_uv = a_uv;
    gl_Position = u_projection * u_model * vec4(a_pos, 0.0f, 1.0f);
}
