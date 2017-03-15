#version 330

in vec2 uv;

out vec3 color;

uniform sampler2D tex;

uniform bool water;

void main() {
    color = texture(tex, uv).rgb;
}
