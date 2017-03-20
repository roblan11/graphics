#version 330

uniform vec3 La, Ld, Ls;
uniform vec3 ka, kd, ks;
uniform float alpha;

in vec3 normal_mv;
in vec3 light_dir;
in vec3 view_dir;

out vec3 color;

void main() {
    color = vec3(0.f);
    //>>>>>>>>>> TODO >>>>>>>>>>>
    // TODO 1.2: Phong shading.
    //<<<<<<<<<< TODO <<<<<<<<<<<

    // 1) compute ambient term.
    color += La * ka;

    vec3 n = normalize(normal_mv);
    vec3 l = normalize(light_dir);
    float lambert = dot(n, l);

    if(lambert > 0.0) {
        // 2) compute diffuse term.
        color += Ld * kd * lambert;

        vec3 v = normalize(view_dir);
        vec3 r = reflect(-l, n);

        // 3) compute specular term.
        color += Ls * ks * pow(max(dot(r, v), 0.0), alpha);
    }
}
