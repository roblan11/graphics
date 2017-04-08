#version 330

in vec2 uv;

out vec3 color;

uniform sampler2D tex;

void main() {
    float temp = texture(tex, uv).x;
    if(temp < 0.01){
        color = vec3(0, 0.6, 0.9);
    } else if (temp < 0.05){
        color = vec3(0.6, 0.7, 0.0);
    } else if (temp < 0.3){
        color = vec3(0.2, 0.7, 0.1);
    } else {
        color = vec3(0.9, 0.9, 0.9);
    }
}
