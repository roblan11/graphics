1 Fast Gaussian Blur ===========================================================

...

2 Screen Space Reflections =====================================================

...

3 Motion Blur ==================================================================

In the fragment shader of the quad, we computed the difference between the current and the last position, and then simply stored the X and Y coordinates in the corresponding value.

For the screen quads fragment shader however, we first extracted the velocity from the texture and stored the X and Y coordinates in a value.
With this value, we then chose a number of samples, and added up the pixels along the velocity vector, with a precision dependent on the number of samples. The values from the pixels were each scaled by a factor, such that the farther away the added pixel is from the position, the smaller it's contribution to the color of the pixel. At the end we divided by the sum of all these factors.

4. Open Ended ==================================================================

OPTIONAL // TODO
