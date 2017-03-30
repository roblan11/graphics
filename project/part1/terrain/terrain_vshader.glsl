#version 330

in vec2 position;

out vec2 uv;
out float altitude;

uniform mat4 MVP;

void main() {
    uv = position;

    float height = 0.3 * (sin(4 * position.x) + cos(5 * position.y));
    vec3 pos_3d = vec3(position.x, height, -position.y);

    altitude = height;
    gl_Position = MVP * vec4(pos_3d, 1.0);
}
