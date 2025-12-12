#version 330 core

layout (location = 0) in vec3 a_pos;
layout (location = 1) in vec2 a_uv;
layout (location = 2) in vec3 a_normal;
layout (location = 3) in float a_opacity;

uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_projection;

out vec2 o_uv;
out vec3 o_normal;
out float o_opacity;

void main() {
    o_uv = a_uv;
    o_normal = a_normal;
    o_opacity = a_opacity;
    mat4 mvp = u_projection * u_view * u_model;
    gl_Position = mvp * vec4(a_pos, 1.0f);
}
