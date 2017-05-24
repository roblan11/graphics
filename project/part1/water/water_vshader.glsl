#version 330 core
uniform mat4 MVP;
in vec3 vpoint;
in vec2 vtexcoord;
out vec2 uv;

const float tiling = 3.0;

void main() {
    gl_Position = MVP * vec4(vpoint, 1.0);
    uv = vec2(vtexcoord.x / 2.0 + 0.5, vtexcoord.y / 2.0 + 0.5) * tiling;
}
