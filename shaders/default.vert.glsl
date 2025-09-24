#version 330 core

layout (location = 0) in vec3 a_pos;
layout (location = 1) in vec2 a_uv;

uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_projection;

out vec2 o_uv;

void main() {
    o_uv = a_uv;
    mat4 mvp = u_projection * u_view * u_model;
    gl_Position = mvp * vec4(a_pos, 1.0f);
}
