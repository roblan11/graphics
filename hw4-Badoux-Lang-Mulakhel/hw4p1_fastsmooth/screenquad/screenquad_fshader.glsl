#version 330

in vec2 uv;

out vec3 color;

uniform sampler2D tex;
uniform float tex_width;
uniform float tex_height;

uniform float kernel[1024];
uniform int kLength;
uniform bool onXaxis;

float rgb_2_luma(vec3 c) {
    return 0.3*c[0] + 0.59*c[1] + 0.11*c[2];
}

void main() {

    //apply the convolution along x and save to tmp texture (don't know how to save to texture)
    vec3 color_tot = vec3(0,0,0);
    float weight_tot = 0;
    int SIZE = kLength >> 1;

    if (onXaxis) {
        for (int i = -SIZE; i <= SIZE; ++i) {
            float w = kernel[SIZE + i];
            vec3 neigh_color = texture(tex, uv+vec2(i/tex_width,0)).rgb;
            color_tot += w * neigh_color;
            weight_tot += w;
        }
    } else {
        for (int i = -SIZE; i <= SIZE; ++i) {
            float w = kernel[SIZE + i];
            vec3 neigh_color = texture(tex, uv+vec2(0,i/tex_width)).rgb;
            color_tot += w * neigh_color;
            weight_tot += w;
        }
    }
    color = color_tot / weight_tot;

}
