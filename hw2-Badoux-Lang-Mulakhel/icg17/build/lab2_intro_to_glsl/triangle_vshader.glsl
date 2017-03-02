#version 330

in vec3 vpoint;
out vec3 fcolor;
#define PI 3.14159265

void main() {
    mat4 r = mat4(1);
    r[0][0] = cos(PI/4);
    r[0][1] = sin(PI/4);
    r[1][0] = -sin(PI/4);
    r[1][1] = cos(PI/4);

    mat4 s = mat4(1);
    s[0][0] = 0.5;
    s[1][1] = 0.7;

    mat4 t = mat4(1);
    t[3][0] = 0.2;
    t[3][1] = 0.5;

    gl_Position = r*s*t*vec4(vpoint, 1.0);

    if(gl_VertexID == 0) {
      fcolor = vec3(1.0, 0.0, 0.0);
    } else if(gl_VertexID == 1) {
      fcolor = vec3(0.0, 1.0, 0.0);
    } else {
      fcolor = vec3(0.0, 0.0, 1.0);
    }
}
