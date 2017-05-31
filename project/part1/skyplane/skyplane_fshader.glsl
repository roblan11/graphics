#version 330 core
out vec3 color;
in vec2 uv;
uniform sampler2D tex_dudv;
uniform sampler2D tex_clouds;

void main() {
    float deep = texture(tex_clouds, uv).x;
    if (deep < -0.2) {
        color = vec3(0.8, 0.8, 0.9);
    } else if (deep < -0.1) {
        color = vec3(0.2, 0.0, 0.9);
    } else if (deep < 0.0) {
        color = vec3(0.8, 0.8, 0.9);
    } else if (deep < 0.1) {
        color = vec3(0.2, 0.0, 0.9);
    } else if (deep < 0.2) {
        color = vec3(0.8, 0.8, 0.9);
    } else if (deep < 0.3) {
        color = vec3(0.2, 0.0, 0.9);
    } else if (deep < 0.4) {
        color = vec3(0.8, 0.8, 0.9);
    } else {
        color = vec3(0.2, 0.0, 0.9);
    }
}
