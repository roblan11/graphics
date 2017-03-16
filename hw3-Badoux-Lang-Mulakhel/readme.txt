1. Phong Shading
================

This part is almost like the one in the exercise of the week. The only difference is the fact we compute the normal vector which is interpolated. That's why we calculate
* normal_mv
* light_dir
* view_dir
in `phong.vshader.glsl`
We pass these vector to the `fshader` where we compute $I$ using the formula given the slide 9 of the file `shading.pdf`. Computing it here will interpolate the pixel normal and therefore give phoning effect.
