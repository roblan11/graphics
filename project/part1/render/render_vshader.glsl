#version 330

in vec2 position;

out vec2 uv;

uniform mat4 MVP;
uniform float time;
uniform bool water;

uniform sampler2D tex;

void main() {
    uv = (position + vec2(1.0, 1.0)) * 0.5;

    float height = texture(tex,uv).x;
    vec3 pos_3d = vec3(position.x, position.y, height);

    gl_Position = MVP * vec4(pos_3d, 1.0);
}
