1. Perspective Projection ===========================================================

Copying the formulas given in the slides of the last lecture, we initialized the necessary variables and then used them to fill in the 4x4 matrix. For this, in order to get a normal base size, we had to divide the FOV by 2.
Finally, to actually use the new projection, we just had to uncomment the given line and comment the corresponding ones related to the Orthographic Projection.

2. Trackball ========================================================================

ProjectionOntoSurface:


Create rotation function:
In main, we simply had to pass the corresponding arguments to the functions in trackball.h, and then multiply the returned result with the matrix from the previously drawn frame.
In trackball.h, we calculated the axis of rotation and the angle. Once that was done, we just passed those two arguments into a rotation, inverting and slightly scaling the rotation direction for a more natural feel.

Zoom:
Translate the z coordinate by the transformed screen coordinate along the y direction, which was calculated prior. To make the zooming less fast and generally more manageable, we divided the transformed coordinates by 10. Additionally, we sped up the zoom by a constant factor.

3. Triangle Grid and Animation ======================================================

Wireframe:
We had to iterate over the square 2-dimensionally, first adding all the vertices and then, in a second step, once all the necessary vertices for a quad have been added into the list, compute the two necessary triangles and add them into the indices.
We had some trouble, as initially it drew some extra lines (due to using GL_TRIANGLE_STRIP instead of GL_TRIANGLES).
After fixing that the triangles were drawn completely incorrectly, or, some of them simply weren't drawn at all. This time the issue was that we still added the vertices as if it was a quad, rather than adding two separate triangles per square.

Animation:
As mentionned in the pdf document, we simply touch the file `grid/grid_vshader.glsl`. We want to have something that looks like waves moving as a function of time. In order to achieve this, we take inspiration from the chessboard of the last week. The main difference is that we are not changing the color but the height.

4. Water animation ==================================================================

To make the animation look like water we colored the waves in blue. We also multiply the height by 2 / time to double the height of the waves and to reduce it over time (because the height of the weight get smaller in a close bucket).

We set a uniform boolean variable "water" to true to start the water animation (line 28 of grid.h).
