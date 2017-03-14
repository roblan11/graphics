#version 330

in vec2 uv;

out vec3 color;

uniform sampler2D tex;

uniform bool water;

void main() {
    if(water){
        color = vec3(0.25, 0.64, 0.87);
    } else {
        color = texture(tex, uv).rgb;
    }
}
