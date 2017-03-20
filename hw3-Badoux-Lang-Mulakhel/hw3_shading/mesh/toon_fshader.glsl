#version 330

uniform vec3 La, Ld, Ls;
uniform vec3 ka, kd, ks;
uniform float alpha;

in vec3 normal_mv;
in vec3 light_dir;
in vec3 view_dir;

out vec3 color;

uniform sampler1D tex1D;

void main() {
    color = vec3(0.0,0.0,0.0);

    ///>>>>>>>>>> TODO >>>>>>>>>>>
    /// TODO 2.2: Toon shading.
    ///<<<<<<<<<< TODO <<<<<<<<<<<

    /// 1) compute ambient term.
    color = La*ka;

    vec3 n = normalize(normal_mv);
    vec3 l = normalize(light_dir);

    // map to 1D texture
    float lambert = texture(tex1D, dot(n,l)).x;

    if(lambert > 0.0) {
        /// 2) compute diffuse term using the texture sampler tex.
        color += Ld*kd*lambert;

        vec3 v = normalize(view_dir);
        vec3 r = reflect(-l,n);

        /// 3) compute specular term using the texture sampler tex.
        color += Ls*ks*max(texture(tex1D, pow(dot(r, v), alpha)).x, 0.0);
    }
}
