#version 330

in vec2 uv;

out vec3 color;

uniform sampler2D tex;

void main() {
    float temp = texture(tex, uv).x;
    if(temp < 0.001){
        color = vec3(0, 0.6, 0.9);
    } else if (temp < 0.02){
        color = vec3(0.6, 0.7, 0.0);
    } else if (temp < 0.07){
        color = mix(vec3(0.6, 0.7, 0.0), vec3(0.2, 0.7, 0.1), (temp-0.02)/0.07);
    } else if (temp < 0.2){
        color = vec3(0.2, 0.7, 0.1);
    } else if (temp < 0.4){
        color = mix(vec3(0.2, 0.7, 0.1), vec3(0.9, 0.9, 0.9), (temp-0.2)/0.2);
    } else {
        color = vec3(0.9, 0.9, 0.9);
    }
}
