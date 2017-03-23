1. Phong Shading ===========================================================================

This part is almost like the one in the exercise of the week. The only difference is the fact we compute the normal vector which is interpolated. That's why we calculate the normal, the light and the view direction in `phong.vshader.glsl`.
We then pass these vectors to the corresponding fshader where we compute I using the formula given in the course slides. Computing it here will interpolate the pixel normal and therefore give a more accurately shaded image.

2. Toon Shading ============================================================================

Here we copy the code of the phong shading part and we map the dot product of the normal and the light direction to the discrete range stored in the given one-dimensional texture using the texture() method to get the shading.
Additionally we checked if the program is currently using the toon shader, and if so bound the texture.

3. Artistic Shading ========================================================================

This works similarly to the toon shader, except that we map the dot product of the normal and the light over the x direction, and the dot product of the reflection and the view to the power alpha over y. All this is mapped over a two-dimensional texture that was bound prior (if the artistic shader is being executed).

4. Flat Shading ============================================================================

computing the light direction and the view direction was done identically as in the phong shader.
To compute the normal of the triangle, we computed the partial derivatives using dFdx and dFdy, and then took the cross product of those two vectors.
With those vectors, we calculated I.

5. Spot Light Shading ======================================================================

The only difference to phong shading is that now we additionally have a light cutoff rather than just a direction. So, we computed the spot effect as the dot product of the light direction and the spot direction.
If that angle is greater than the spot cutoff angle, so if the current dot is inside the light cone, we compute I. Otherwise, we leave the light at 0 (black). At the end, we multiply I with the contribution by the spot light, which is the previously spot effect to the power of a pre-defined exponent.
