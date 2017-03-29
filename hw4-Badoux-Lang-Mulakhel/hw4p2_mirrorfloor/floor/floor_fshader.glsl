#version 330 core
out vec3 color;
in vec2 uv;
uniform sampler2D tex;
uniform sampler2D tex_mirror;

void main() {
    /// TODO: query window_width/height using the textureSize(..) function on tex_mirror
    ivec2 tex_size = textureSize(tex_mirror, 0);
    float window_width = float(tex_size.x);
    float window_height = float(tex_size.y);

    /// TODO: use gl_FragCoord to build a new [_u,_v] coordinate to query the framebuffer
    float _u = gl_FragCoord.x / window_width ;
    float _v = 1.f - gl_FragCoord.y / window_height;

    /// TODO: mix the texture(tex,uv).rgb with the value you fetch by texture(tex_mirror,vec2(_u,_v)).rgb
    vec3 color_from_texture = texture(tex, uv).rgb;
    vec3 color_from_mirror = texture(tex_mirror, vec2(_u, _v)).rgb;
    color = mix(color_from_texture, color_from_mirror, vec3(0.15));
}
