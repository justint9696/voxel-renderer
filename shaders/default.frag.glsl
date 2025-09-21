#version 330 core

in vec2 o_uv;

uniform vec4 u_color;

uniform sampler2D u_texture;

out vec4 frag_color;

void main() {
    frag_color = mix(texture(u_texture, o_uv), u_color, 0.15f);
}
