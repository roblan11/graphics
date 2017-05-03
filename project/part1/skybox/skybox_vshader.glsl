#version 330

in vec3 vpoint;
in vec2 vtexcoord;

out vec2 uv;

uniform mat4 MVP;

void main() {
    // spin-circ
    gl_Position =  MVP * vec4(vpoint,1);

    uv = vtexcoord;
}
