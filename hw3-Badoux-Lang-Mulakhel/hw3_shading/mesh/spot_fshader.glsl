#version 330

uniform vec3 La, Ld, Ls;
uniform vec3 ka, kd, ks;
uniform float alpha;
uniform vec3 spot_dir;

in vec3 normal_mv;
in vec3 light_dir;
in vec3 view_dir;

out vec3 color;

const float spot_cos_cutoff = 0.985; // cos 10 degrees
const float spot_exp = 150;

void main() {

    //>>>>>>>>>> TODO >>>>>>>>>>>
    // TODO 5: Spot light.
    // Complete the shader to obtain spot light effect
    //<<<<<<<<<< TODO <<<<<<<<<<<

    // initialize color to 0 (black outside the cone)
    color = vec3(0.f);

    // normal vector
    vec3 n = normalize(normal_mv);

    // light direction
    vec3 l = normalize(light_dir);

    // spot direction
    vec3 s = normalize(spot_dir);

    float lambert = dot(n, l);
    float spot_effect = dot(l, s);

    // only compute color if inside the cone
    if(spot_effect > spot_cos_cutoff){
        // ambient
        color += La * ka;

        if(lambert > 0.f){
            // diffuse
            color += Ld * kd * lambert;

            vec3 v = normalize(view_dir);
            vec3 r = reflect(-l,n);

            // specular
            color += Ls * ks * pow(max(dot(r, v), 0.0), alpha);
        }

        // add the contribution of the spot light
        color *= pow(spot_effect, spot_exp);
    }
}
