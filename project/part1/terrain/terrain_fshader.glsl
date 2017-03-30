#version 330

in vec2 uv;
in float altitude;

out vec3 color;

uniform sampler2D tex;

void main() {
    //vec3 dx = dFdx(vpoint_mv.xyz);
    //vec3 dy = dFdy(vpoint_mv.xyz);
    //vec3 n = normalize(cross(dx, dy));

    if (altitude > 0.2) {
        color = vec3(0.9, 0.9, 0.9);
    } else if (altitude > -0.1){
        color = vec3(0, 1, 0);
    } else {
        color = vec3(0, 0, 1);
    }

}
