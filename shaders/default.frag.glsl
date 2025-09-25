#version 330 core

in vec2 o_uv;
in vec3 o_normal;

uniform vec4 u_color;
uniform sampler2D u_texture;

out vec4 frag_color;

void main() {
    vec3 light = vec3(0.5f, 1.0f, 0.75f);
    float diff = max(abs(dot(o_normal, light)), 0.0f);
    vec3 sample = texture(u_texture, o_uv).rgb;
    vec3 diffuse = diff * sample;
    frag_color = vec4(mix(diffuse, u_color.rgb, 0.1f), 1.0f);
}
