#version 330

in vec3 vpoint;
out vec3 fcolor;

uniform mat4 ROTATE;
uniform mat4 SCALE;
uniform mat4 TRANSLATE;

void main() {
    gl_Position = ROTATE * SCALE * TRANSLATE * vec4(vpoint, 1.0);

    if(gl_VertexID == 0) {
      fcolor = vec3(1.0, 0.0, 0.0);
    } else if(gl_VertexID == 1) {
      fcolor = vec3(0.0, 1.0, 0.0);
    } else {
      fcolor = vec3(0.0, 0.0, 1.0);
    }
}
