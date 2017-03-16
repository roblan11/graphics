1.1 Triangle Spirals ================================================================

Spiral:
Iterating over n, at every iteration a triangle is drawn, where the angle of its rotation, as well as its scale and distance from the center depend on n.
The triangles grow and go further outward, while the rotation was chosen such that 10 triangles are drawn in a full rotation.
The translation is only done in the direction of the positive x axis (right), as it will simply be rotated around the center afterwards, which creates the wanted effect.

Fermat’s Spiral:
Firstly: To switch between the Spiral and the Fermat's spiral we created an enum and a variable to store the current mode.
Then, for the actual Fermat's Spiral, with the values provided in the Lab2 slides and the formulas from Wikipedia, it was just about implementing the algorithm with some number of triangles and the distance between them (done through Translation).

1.2 Checkerboard ====================================================================

The change of color is achieved through mapping a 3 dimensional graph (in this case a 3D sine wave) to some colors. In this implementation, if the third dimension is 0 the color is red (RGB = 1,0,0), at 1 it's green (RGB = 0,1,0) and at 0.5 it's yellow (RGB = 1,1,0), with interpolated colors in between.

The 3D sine wave is done by multiplying two sine waves (one over x, the other over y), and then dividing the result by 2 and adding 0.5. This is done because the multiplication of 2 sine waves has results in [-1, 1], whereas we want the results to be in [0, 1].

To achieve enough oscillations on the sines, the x and y coordinates are multiplied by a multiple of π.

1.3 Solar System ====================================================================

Three models need to be computed: the ones of the sun, earth and moon.

The scales were chosen in a way such that sun > earth > moon, while keeping all of them big enough such that the rotation is still visible.

As for the rotations:
- The moon does not rotate in relation to the earth (the same side of the moon is always visible from earth).
- Earth rotates fastest, which for simplicity is 2 times the rotation speed of the moon.
- The sun rotates slowest, for simplicity 4 times slower than earth or half as fast as the moon

Lastly the translations:
- The sun was translated to the right as shown in the example in the PDF.
- Earth is translated in a way such that its trajectory is an ellipse. In order to simplify this task it is first rotated and then translated, such that the translation can be done using the standard formula that for an ellipse.
- The moon is translated relative to the earth, as it's supposed to rotate around it. For this, it first gets translated to the position of the earth and then along a circle.

1.4 Arkanoid ========================================================================

The goal of this part was to use the input from the keyboard. The game is really basic and it can be better coded if:
- we'd use use a class Ball instead of a quad
- we'd replace the pad quad by a class Pad.
Doing this way, we can specify a different color for each of the two objects and remove global variables from the code.

The implementation was mostly copy-pasted code from the previous exercises, with some different parameters to adjust the size and add movement.
