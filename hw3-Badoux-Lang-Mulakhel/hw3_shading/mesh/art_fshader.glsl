#version 330

uniform vec3 La, Ld, Ls;
uniform vec3 ka, kd, ks;

in vec3 normal_mv;
in vec3 light_dir;
in vec3 view_dir;

out vec3 color;

uniform float alpha;
uniform sampler2D tex2D;

void main() {
    color = vec3(0.0, 0.0, 0.0);

    //>>>>>>>>>> TODO >>>>>>>>>>>
    // TODO 3.2: Artistic shading.
    // 1) compute the output color by doing a look-up in the texture using the
    //    texture sampler tex.
    //<<<<<<<<<< TODO <<<<<<<<<<<

    color = La*ka;
    vec3 n = normalize(normal_mv);
    vec3 l = normalize(light_dir);
    vec3 v = normalize(view_dir);
    vec3 r = reflect(-l,n);
    vec3 value = texture(tex2D, vec2(dot(n,l), pow(max(dot(r,v), 0.0), alpha))).xyz;
    color += Ld*kd*value;
    color += Ls*ks*value;
}
