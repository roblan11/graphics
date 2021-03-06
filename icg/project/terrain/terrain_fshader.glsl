#version 330

in vec4 vpoint_mv;
in vec3 light_dir, view_dir;
in vec2 uv;
uniform float position_looking_at_x;
uniform float position_looking_at_y;
in float height;

out vec3 color;

uniform sampler2D uw_texture;
uniform sampler2D rock_texture;
uniform sampler2D sand_texture;
uniform sampler2D grass_texture;
uniform sampler2D snow_texture;
uniform mat4 MV;

uniform vec3 La, Ld;
uniform vec3 ka, kd;
uniform float alpha;

uniform float mix_uw_sand;
uniform float mix_sand_grass;
uniform float lvl_grass;
uniform float mix_grass_rock;
uniform float lvl_rock;
uniform float mix_rock_snow;
uniform float lvl_snow;

uniform float scale_uw;
uniform float scale_sand;
uniform float scale_grass;
uniform float scale_rock;
uniform float scale_snow;

uniform float normal_contribution;
uniform float normal_contribution_mix;

void main() {
    vec2 xy = uv;
    xy.x += position_looking_at_x;
    xy.y += position_looking_at_y;

    vec3 x = dFdx(vpoint_mv.xyz);
    vec3 y = dFdy(vpoint_mv.xyz);
    vec3 n = normalize(cross(x, y) * mat3(MV));

    float z = (1-n.z);

    vec3 tex_color = vec3(0.0);
    if(height < -mix_uw_sand) {
        tex_color = texture(uw_texture, xy*scale_uw).xyz;
    } else if(height < mix_uw_sand) {
        tex_color = mix(texture(uw_texture, xy*scale_uw).xyz, texture(sand_texture, xy*scale_sand).xyz, (height+mix_uw_sand)/(2*mix_uw_sand));
    } else if (height < mix_sand_grass) {
        tex_color = texture(sand_texture, xy*scale_sand).xyz;
    } else if (height < lvl_grass) {
        tex_color = mix(texture(sand_texture, xy*scale_sand).xyz, texture(grass_texture, xy*scale_grass).xyz, (height-mix_sand_grass)/(lvl_grass-mix_sand_grass));
    } else if (height < mix_grass_rock) {
        tex_color = texture(grass_texture, xy*scale_grass).xyz;
    } else if (height < lvl_rock) {
        tex_color = mix(texture(grass_texture, xy*scale_grass).xyz, texture(rock_texture, xy*scale_rock).xyz, (height-mix_grass_rock)/(lvl_rock-mix_grass_rock));
    } else if ( (height < mix_rock_snow) || (z > (normal_contribution+normal_contribution_mix)) ) {
        tex_color = texture(rock_texture, xy*scale_rock).xyz;
    } else if(z>normal_contribution) {
        float factor1 = (1-(z-normal_contribution)/normal_contribution_mix);
        float factor = (height-mix_rock_snow)/(lvl_snow-mix_rock_snow)*clamp(factor1, 0.0, 1.0);
        tex_color = mix(texture(rock_texture, xy*scale_rock).xyz, texture(snow_texture, xy*scale_snow).xyz, factor);
    } else if (height < lvl_snow) {
        tex_color = mix(texture(rock_texture, xy*scale_rock).xyz, texture(snow_texture, xy*scale_snow).xyz, (height-mix_rock_snow)/(lvl_snow-mix_rock_snow));
    } else {
        tex_color = texture(snow_texture, xy*scale_snow).xyz;
    }

    vec3 gray = vec3(0.f);
    // ambient
    gray += La * ka;

    vec3 l = normalize(light_dir);
    float lambert = dot(n, l);

    if(lambert > 0.0) {
        // diffuse
        gray += Ld * kd * lambert;

    }
    color = tex_color * gray;
}
