1. Phong Shading ===========================================================================

This part is almost like the one in the exercise of the week. The only difference is the fact we compute the normal vector which is interpolated. That's why we calculate
* normal_mv
* light_dir
* view_dir
in `phong.vshader.glsl`
We pass these vector to the `fshader` where we compute $I$ using the formula given the slide 9 of the file `shading.pdf`. Computing it here will interpolate the pixel normal and therefore give phoning effect.

2. Toon Shading ============================================================================

Here we copy the code of the phong shading part and we map the dot(n,l) and dot(r,v) to the discrete range stored in the given 1d texture using the texture() method to get the shading.