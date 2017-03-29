1. Fast Gaussian Blur ===========================================================

In order to change the blur level, use the key q or w. There is a fixed range where the value of standart deviation is allowed to be. Now the computation of the matrix is done in the cpu (screenquad.h), then it is send to the gpu.

We do the computation in two passes. Firstly, we work only on the x axis. Secondly, we work on the y axis. (When we try to do it at the same time, we have a white image)

2. Screen Space Reflections =====================================================

In main we mirrored the camera (taking the negative z coordinate of the camera position) and used that vector to compute the new view projection matrix. We render the cube to a framebuffer using this matrix, and the cube and the floor to the screen using the original matrix.

In the fragment shader of the floor we query the mirrored cube texture from the framebuffer (the coordinates are computed using gl_FragCoord, the window relative coordinate of the fragment, and the width and height of the window) and blend it with the texture of the floor.

3. Motion Blur ==================================================================

In the fragment shader of the quad, we computed the difference between the current and the last position, and then simply stored the X and Y coordinates in the corresponding value.

For the screen quads fragment shader however, we first extracted the velocity from the texture and stored the X and Y coordinates in a value.
With this value, we then chose a number of samples, and added up the pixels along the velocity vector (scaled down by the number of samples and a constant value, in order to keep the blur relatively small and realistic). The values from the pixels were given a weight, such that the farther away that pixel is from the current position, the smaller it's contribution to the color of the pixel. At the end we divided by the sum of all weights.
