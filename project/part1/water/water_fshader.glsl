#version 330 core
out vec3 color;
in vec2 uv;
uniform sampler2D tex;
uniform sampler2D tex_mirror;

uniform float moveFactor;

const float waveStrength = 0.02;

void main() {
    ivec2 tex_size = textureSize(tex_mirror, 0);
    float window_width = float(tex_size.x);
    float window_height = float(tex_size.y);

    float _u = gl_FragCoord.x / window_width ;
    float _v = 1.f - gl_FragCoord.y / window_height;
    vec2 reflectionCoords = vec2(_u, _v);

    vec2 distortionX = (texture(tex, vec2(uv.x + moveFactor, uv.y)).rg * 2.0 - 1.0) * waveStrength;
    vec2 distortionY = (texture(tex, vec2(uv.x, uv.y + moveFactor)).rg * 2.0 - 1.0) * waveStrength;
    vec2 distortion = distortionX + distortionY;
    reflectionCoords += distortion;
    //reflectionCoords.x = clamp(reflectionCoords.x, 0.001, 0.999);
    //reflectionCoords.y = clamp(reflectionCoords.y, -0.999, -0.001);

    vec3 color_from_texture = texture(tex, reflectionCoords).rgb;
    vec3 color_from_mirror = texture(tex_mirror, reflectionCoords).rgb;
    color = mix(vec3(0.0, 0.0, 1.0), vec3(color_from_texture.x/5, color_from_texture.y/3, color_from_texture.z), 0.5);
}
