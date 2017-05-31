#version 330 core
out vec3 color;
in vec2 uv;
uniform sampler2D tex_dudv;
uniform sampler2D tex_clouds;
uniform sampler2D tex_cloud;
uniform sampler2D tex_sky;

void main() {
    float deep = texture(tex_clouds, uv).x;
    if (deep < -0.2) {
        color = texture(tex_sky, uv).xyz;
    } else if (deep < -0.1) {
        color = texture(tex_cloud, uv).xyz;
    } else if (deep < 0.0) {
        color = texture(tex_sky, uv).xyz;
    } else if (deep < 0.1) {
        color = texture(tex_cloud, uv).xyz;
    } else if (deep < 0.2) {
        color = texture(tex_sky, uv).xyz;
    } else if (deep < 0.3) {
        color = texture(tex_cloud, uv).xyz;
    } else if (deep < 0.4) {
        color = texture(tex_sky, uv).xyz;
    } else {
        color = texture(tex_cloud, uv).xyz;
    }
}
