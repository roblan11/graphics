1. Perspective Projection ===========================================================

Copying the formulas given in the slides of the last lecture, we initialized the necessary variables and then used them to fill in the 4x4 matrix.
Finally, to actually use the new projection, we just had to uncomment the given line and comment the corresponding ones related to the Orthographic Projection.

!! ADJUST SCALE ?

2. Trackball ========================================================================

!! USE DX / DY INSTEAD OF X, Y TO AVOID SNAPPING WHEN PLACING MOUSE DOWN

ProjectionOntoSurface:


Create rotation function:


Zoom:
Translate the z coordinate by the transformed screen coordinate along the y direction, which was calculated prior. To make the zooming less fast and generally more manageable, we divided the transformed coordinates by 10.

3. Zooming ==========================================================================

!! TODO

4. Triangle Grid ====================================================================

!! TODO
