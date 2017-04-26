#version 330

in vec4 vpoint_mv;
in vec3 light_dir, view_dir;
in vec2 uv;
in float height;

out vec3 color;

uniform sampler2D tex;
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
    if(height < lvl_sand){
        color = vec3(0, 0.6, 0.9);
    } else if (height < mix_sand_grass){
        color = vec3(0.6, 0.7, 0.0);
    } else if (height < lvl_grass){
        color = mix(vec3(0.6, 0.7, 0.0), vec3(0.2, 0.7, 0.1), (height-mix_sand_grass)/(lvl_grass-mix_sand_grass));
    } else if (height < mix_grass_snow){
        color = vec3(0.2, 0.7, 0.1);
    } else if (height < lvl_snow){
        color = mix(vec3(0.2, 0.7, 0.1), vec3(0.7, 0.7, 0.7), (height-mix_grass_snow)/(lvl_snow-mix_grass_snow));
    } else {
        color = vec3(0.7, 0.7, 0.7);
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

        /// 3) compute specular term.
        if(height < lvl_sand) {
          gray += Ls * ks * pow(max(dot(r, v), 0.0), alpha);
        }
    }
    color = color * gray;
    //color = n;
}
