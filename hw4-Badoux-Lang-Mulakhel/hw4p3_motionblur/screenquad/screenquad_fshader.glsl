#version 330 core
in vec2 uv;
uniform sampler2D colorTex;
uniform sampler2D velocityTex;
out vec4 color;

void main() {
    color = vec4(0.f, 0.f, 0.f, 1.f);

    /// TODO: use the velocity vector stored in velocityTex to compute the line integral
    vec2 vel = texture(velocityTex, uv).xy;


    /// TODO: use a constant number of samples for integral (what happens if you take too few?)
    const int samples = 10;
    int sum = 0;
    for(int i = 0; i <= samples; i++) {
        vec4 value = texture(colorTex, uv + i * vel / samples);
        int factor = samples - i;
        color += value * factor;
        sum += factor;
    }

    color /= sum;

}
