#version 330

in vec2 uv;

out vec3 color;

uniform sampler2D tex;

void main() {
    float temp = texture(tex,uv).x;
    if(temp < 0.3){
        color = vec3(0, 191, 255);
    } else if (temp < 0.9){
        color = vec3(69, 139, 0);
    } else {
        color = vec3(255, 255, 255);
    }
}
