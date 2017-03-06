1.1 Triangle Spirals ================================================================

Spiral:
After initializing the matrices and declaring the important entries it was just about
finding fitting values for scaling, translation and rotation.

Fermat’s Spiral:
To switch between the Spiral and the Fermat's spiral we created an enum and a variable
to store the current mode.
Then, for the actual Fermat's Spiral, with the values provided in the Lab2 slides and
the formulas from Wikipedia, it was just about implementing the algorithm with some
number of triangles and the distance between them (done through Translation).

1.2 Checkerboard ====================================================================

As specified in the (long) pdf describing the homework and the exercises, we were told
to update only the fshader file. The value of ux.x and ux.y range from [0:1]. When
the value is 0.5, we have yellow. Red or green when it is under or over it.
1. So we use a sine function to get a periodicity. It maps from -1 to 1.
2. we divide by 2 to map it to -0.5 to 0.5.
3. then we add 0.5 to map it to 0 to 1

We also include a angular velocity to have the number of changes desired.
Since we are in 2D. We do it one time in horizontal and vertical and multiply the
product of the sine.

1.3 Solar System ====================================================================

An ellipse is by definition composed of 3 values a, b and c - with a ≥ b and Kepler's
Law says that the sun is on c while the earth runs on (a, b), s.t. c = sqrt(a² - b²).

1.4 Arkanoid ========================================================================

---
