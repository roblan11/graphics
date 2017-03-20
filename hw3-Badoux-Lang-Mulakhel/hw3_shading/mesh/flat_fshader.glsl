#version 330

in vec4 vpoint_mv;
in vec3 light_dir, view_dir;

out vec3 color;

uniform vec3 La, Ld, Ls;
uniform vec3 ka, kd, ks;
uniform float alpha;


void main() {
    color = vec3(0.0, 0.0, 0.0);

    ///>>>>>>>>>> TODO >>>>>>>>>>>
    /// TODO 4.2: Flat shading.
    ///<<<<<<<<<< TODO <<<<<<<<<<<

    /// 1) compute triangle normal using dFdx and dFdy
    vec3 x = dFdx(vpoint_mv.xyz);
    vec3 y = dFdy(vpoint_mv.xyz);
    vec3 n = normalize(cross(x, y));

    /// 1) compute ambient term.
    color += La * ka;

    vec3 l = normalize(light_dir);
    float lambert = dot(n, l);

    if(lambert > 0.0) {
        /// 2) compute diffuse term.
        color += Ld * kd * lambert;

        vec3 v = normalize(view_dir);
        vec3 r = reflect(-l, n);

        /// 3) compute specular term.
        color += Ls * ks * pow(max(dot(r, v), 0.0), alpha);
    }
}
