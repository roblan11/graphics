#version 330

in vec4 vpoint_mv;
in vec3 light_dir, view_dir;
in vec2 uv;
in float height;

out vec3 color;

uniform sampler2D tex;
uniform sampler2D rock_texture;
uniform sampler2D sand_texture;
uniform sampler2D grass_texture;
uniform sampler2D snow_texture;
uniform mat4 MV;
uniform vec3 La, Ld, Ls;
uniform vec3 ka, kd, ks;
uniform float alpha;

uniform float lvl_sand;
uniform float mix_sand_grass;
uniform float lvl_grass;
uniform float mix_grass_snow;
uniform float lvl_snow;

void main() {
    if(height < 0){
        color = vec3(0.2, 0.2, 0.2);
    } else if (height < mix_sand_grass){
        color = vec3(0.6, 0.7, 0.0);
    } else if (height < 0.07){
        color = mix(vec3(0.6, 0.7, 0.0), vec3(0.2, 0.7, 0.1), (height-0.02)/0.07);
    } else if (height < 0.2){
        color = vec3(0.2, 0.7, 0.1);
    } else if (height < 0.4){
        color = mix(vec3(0.2, 0.7, 0.1), vec3(0.7, 0.7, 0.7), (height-0.2)/0.2);
    } else {
        color = vec3(0.7, 0.7, 0.7);
    }

    /*float delta = 0.03;
    float dx = (texture(tex, vec2(uv.x+delta, uv.y)).x - texture(tex, vec2(uv.x-delta, uv.y)).x);
    float dy = (texture(tex, vec2(uv.x, uv.y + delta)).x - texture(tex, vec2(uv.x, uv.y - delta)).x);
    vec3 direction = vec3(5*(-dx), 5*(-dy), 1.f);
    vec3 n = normalize(direction);*/

    if(height < 0.001){
        color = vec3(0, 0.6, 0.9);
    } else if (height < 0.02){
        color = texture(sand_texture, uv*10).xyz;
    } else if (height < 0.05){
        color = mix(texture(sand_texture, uv*10).xyz, texture(grass_texture, uv*10).xyz, (height-0.02)/0.03);
    } else if (height < 0.15){
        color = texture(grass_texture, uv*10).xyz;
    } else if (height < 0.25){
        color = mix(texture(grass_texture, uv*10).xyz, texture(rock_texture, uv*5).xyz, (height-0.15)/0.1);
    } else if (height < 0.35){
        color = texture(rock_texture, uv*5).xyz;
    } else if (height < 0.4) {
        color = mix(texture(rock_texture, uv*5).xyz, texture(snow_texture, uv*5).xyz, (height-0.35)/0.05);
    } else {
        color = texture(snow_texture, uv*5).xyz;
    }

    vec3 x = dFdx(vpoint_mv.xyz);
    vec3 y = dFdy(vpoint_mv.xyz);
    vec3 n = normalize(cross(x, y) * mat3(MV));

    vec3 gray = vec3(0.f);
    /// 1) compute ambient term.
    gray += La * ka;

    vec3 l = normalize(light_dir);
    float lambert = dot(n, l);

    if(lambert > 0.0) {
        /// 2) compute diffuse term.
        gray += Ld * kd * lambert;

        vec3 v = normalize(view_dir);
        vec3 r = reflect(-l, n);

    }
    //color = n;
    color = color * gray;
}
