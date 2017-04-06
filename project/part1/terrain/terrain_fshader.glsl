#version 330

in vec2 uv;

out vec3 color;

uniform sampler2D tex;

void main() {
    float temp = texture(tex,uv).x;
    if(temp < 0.1){
        color = vec3(0, 0.6, 0.9);
    } else if (temp < 0.7){
        color = vec3(0.1, 0.7, 0.7);
    } else {
        color = vec3(1.0, 1.0, 1.0);
    }
}
