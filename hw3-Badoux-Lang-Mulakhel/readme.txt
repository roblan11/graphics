1. Perspective Projection ===========================================================

Copying the formulas given in the slides of the last lecture, we initialized the necessary variables and then used them to fill in the 4x4 matrix. For this, in order to get a normal base size, we had to divide the FOV by 2.
Finally, to actually use the new projection, we just had to uncomment the given line and delete the corresponding ones related to the Orthographic Projection.

2. Trackball ========================================================================

ProjectionOntoSurface:


Create rotation function:
In main, we simply had to pass the corresponding arguments to the functions in trackball.h, and then multiply the returned result with the matrix from the previously drawn frame.
In trackball.h, we calculated the axis of rotation and the angle. Once that was done, we passed those two arguments to make a standard rotation, inverting and slightly scaling the rotation direction for a more natural feel.

Zoom:
We translated the z coordinate by delta y (the difference between the y position of the mouse in the current and the previous frame), which was calculated prior. Additionally, we sped up the zoom by a constant factor, making it faster to zoom in and out without having to move the mouse too much.

3. Triangle Grid and Animation ======================================================

Wireframe:
We iterated over the square two dimensionally, first adding all the vertices and then, in a second step, once all the necessary vertices for a quad have been added into the list, computed the two necessary triangles and added their indices.
We had some trouble, as initially it drew some extra lines (due to using GL_TRIANGLE_STRIP instead of GL_TRIANGLES).

Animation:
In order to achieve this exercise, we took inspiration from the chessboard from last week, while changing the height of each vertice rather than its color. To make the movements two dimensional we multiplied the sines over the x and y direction (both depending on the time), and then scaled the result by a constant to make the resulting waves smaller.

4. Water animation ==================================================================

To make the animation look more like water we added another, bigger and unidirectional wave to the current ones. we used a modulo function to create a way to only draw certain parts of the sine wave (namely the wave between two points where sin(x) = -1). Then, we added one to this sine wave to only have positive values and scaled the result down once again to make it look more natural.
The frequency of the sine was multiplied by 2 to avoid graphical glitches while having the waves appear relatively frequently.
To toggle the water effect, there is a defined boolean value at the beginning of grid.h. Setting this value to false will disable the effect, setting it to true will enable it.
