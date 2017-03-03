#version 330

in vec2 uv;

out vec3 color;

uniform sampler1D colormap;

void main() {
    float omega = 8 * 3.14;
    float x = sin(omega * uv.x);
    float y = sin(omega * uv.y);
    float value = 0.5 * (x * y) + 0.5;
    color = texture(colormap, value).rgb;
}


