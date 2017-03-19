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
    color = vec3(0.f);
    vec3 n = normalize(normal_mv);
    vec3 l = normalize(light_dir);
    vec3 s = normalize(spot_dir);

    float lambert = dot(n,l);
    float spot_effect = dot(l, s);

    if(spot_effect > spot_cos_cutoff){
      if(lambert > 0.0) {
          color += La*ka + Ld*kd*lambert;
          vec3 v = normalize(view_dir);
          vec3 r = reflect(-l,n);
          color += Ls*ks*pow(max(dot(r,v), 0.0), alpha);
      }
      color *= pow(spot_effect, spot_exp);
    }
}
