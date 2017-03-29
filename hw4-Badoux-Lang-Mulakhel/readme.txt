1 Fast Gaussian Blur ===========================================================

...

2 Screen Space Reflections =====================================================

In main we mirrored the camera (the z coordinate of cam_pos is multiplied by -1) and used the new cam_pos to compute the new view projection matrix. We render the cube to a frameshader using this matrix, and the cube and the floor to the screen using the original matrix.

In the floor shader we query the mirrored cube texture from the framebuffer (the coordinates are computed using gl_FragCoord, the window relative coordinate for the fragment, and the width and height of the window) and blend it with the texture of the floor.

3 Motion Blur ==================================================================

In the fragment shader of the quad, we computed the difference between the current and the last position, and then simply stored the X and Y coordinates in the corresponding value.

For the screen quads fragment shader however, we first extracted the velocity from the texture and stored the X and Y coordinates in a value.
With this value, we then chose a number of samples, and added up the pixels along the velocity vector, with a precision dependent on the number of samples. The values from the pixels were each scaled by a factor, such that the farther away the added pixel is from the position, the smaller it's contribution to the color of the pixel. At the end we divided by the sum of all these factors.

4. Open Ended ==================================================================

OPTIONAL // TODO
