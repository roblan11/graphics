#version 330

in vec2 uv;

out vec3 color;

uniform sampler2D tex;
uniform float tex_width;
uniform float tex_height;

uniform float kernel[512];
uniform int kLength;
uniform bool onXaxis;

float rgb_2_luma(vec3 c) {
    return 0.3*c[0] + 0.59*c[1] + 0.11*c[2];
}

void main() {

    vec3 color_tot = vec3(0, 0, 0);
    float weight_tot = 0;
    int SIZE = kLength >> 1;

    if (onXaxis) {
        // apply the convolution along the x axis
        for (int i = -SIZE; i <= SIZE; ++i) {
            float w = kernel[SIZE + i];
            vec3 neigh_color = texture(tex, uv+vec2(i/tex_width, 0)).rgb;
            color_tot += w * neigh_color;
            weight_tot += w;
        }
    } else {
        // apply the convolution along the y axis
        for (int i = -SIZE; i <= SIZE; ++i) {
            float w = kernel[SIZE + i];
            vec3 neigh_color = texture(tex, uv+vec2(0, i/tex_width)).rgb;
            color_tot += w * neigh_color;
            weight_tot += w;
        }
    }
    color = color_tot / weight_tot;

}
