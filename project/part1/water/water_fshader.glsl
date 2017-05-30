#version 330 core
out vec4 color;
in vec2 uv;
in vec3 toCameraVector;
uniform sampler2D tex_dudv;
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

    vec2 distortionX = (texture(tex_dudv, vec2(uv.x + moveFactor, uv.y)).rg * 2.0 - 1.0) * waveStrength;
    vec2 distortionY = (texture(tex_dudv, vec2(uv.x, uv.y + moveFactor)).rg * 2.0 - 1.0) * waveStrength;
    vec2 distortion = distortionX + distortionY;
    reflectionCoords += distortion;

    /*
    vec3 viewVector = normalize(toCameraVector);
    float refractiveFactor = dot(viewVector, vec3(0.0, 0.0, 1.0));
    if (refractiveFactor > 0.01) {
        refractiveFactor = 0.0;
    } else {
        refractiveFactor = 1.0;
    }
    */

    vec3 color_from_mirror = texture(tex_mirror, reflectionCoords).rrr;
    vec3 mix_color = mix(vec3(0.0, 0.3, 0.5), color_from_mirror, 0.2);
    color = vec4(mix(vec3(0.0, 0.0, 1.0), mix_color, 0.5), 0.7);
}
