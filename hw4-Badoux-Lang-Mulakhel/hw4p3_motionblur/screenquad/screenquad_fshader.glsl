#version 330 core
in vec2 uv;
uniform sampler2D colorTex;
uniform sampler2D velocityTex;
out vec4 color;

void main() {
    color = vec4(0.f, 0.f, 0.f, 1.f);

    const int samples = 10;
    const float velocity_scale = 1.f / (1.75 * samples);

    /// TODO: use the velocity vector stored in velocityTex to compute the line integral
    vec2 velocity = velocity_scale * texture(velocityTex, uv).xy;

    /// TODO: use a constant number of samples for integral (what happens if you take too few?)
    int weight_tot = 0;
    for(int i = 0; i < samples; i++) {
        vec4 neigh_color = texture(colorTex, uv + i*velocity);
        int weight = samples - i;
        color += neigh_color * weight;
        weight_tot += weight;
    }

    color /= weight_tot;

}
