#version 330

in vec4 vpoint_mv;
in vec3 light_dir, view_dir;
in vec2 uv;

out vec3 color;

uniform sampler2D tex;
uniform mat4 MV;
uniform vec3 La, Ld, Ls;
uniform vec3 ka, kd, ks;
uniform float alpha;

void main() {
    float temp = texture(tex, uv).x;
    if(temp < 0.001){
        color = vec3(0, 0.6, 0.9);
    } else if (temp < 0.02){
        color = vec3(0.6, 0.7, 0.0);
    } else if (temp < 0.07){
        color = mix(vec3(0.6, 0.7, 0.0), vec3(0.2, 0.7, 0.1), (temp-0.02)/0.07);
    } else if (temp < 0.2){
        color = vec3(0.2, 0.7, 0.1);
    } else if (temp < 0.4){
        color = mix(vec3(0.2, 0.7, 0.1), vec3(0.7, 0.7, 0.7), (temp-0.2)/0.2);
    } else {
        color = vec3(0.7, 0.7, 0.7);
    }

    vec3 x = dFdx(vpoint_mv.xyz);
    vec3 y = dFdy(vpoint_mv.xyz);
    vec3 n = normalize(cross(x, y));

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
        gray += Ls * ks * pow(max(dot(r, v), 0.0), alpha);
    }
    color = color * gray;
}
