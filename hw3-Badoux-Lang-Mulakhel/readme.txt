1. Perspective Projection ===========================================================

Copying the formulas given in the slides of the last lecture, we initialized the necessary variables and then used them to fill in the 4x4 matrix.
Finally, to actually use the new projection, we just had to uncomment the given line and comment the corresponding ones related to the Orthographic Projection.

!! ADJUST SCALE ?

2. Trackball ========================================================================

ProjectionOntoSurface:


Create rotation function:
!! USE DX / DY INSTEAD OF X, Y TO AVOID SNAPPING WHEN PLACING MOUSE DOWN

Zoom:
Translate the z coordinate by the transformed screen coordinate along the y direction, which was calculated prior. To make the zooming less fast and generally more manageable, we divided the transformed coordinates by 10.

3. Triangle Grid and Animation ======================================================

Wireframe:
We had to iterate over the square 2-dimensionally, first adding all the vertices and then, in a second step, once all the necessary vertices for a quad have been added into the list, compute the two necessary triangles and add them into the indices.
We had some trouble, as initially it drew some extra lines (due to using GL_TRIANGLE_STRIP instead of GL_TRIANGLES).
After fixing that the triangles were drawn completely incorrectly, or, some of them simply weren't drawn at all. This time the issue was that we still added the vertices as if it was a quad, rather than adding two separate triangles per square.

Animation:
!! TODO

4. Water animation ==================================================================

!! TODO (bonus)
