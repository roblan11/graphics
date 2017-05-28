#version 330 core
uniform mat4 MVP;
in vec3 vpoint;
in vec2 vtexcoord;
out vec2 uv;

uniform sampler2D tex_clouds;

void main() {
    gl_Position = MVP * vec4(vpoint, 1.0);
    uv = vec2(vtexcoord.x, vtexcoord.y);
}
