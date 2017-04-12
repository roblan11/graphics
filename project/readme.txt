 /¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯\
|                      INTRODUCTION TO COMPUTER GRAPHICS:                      |
|                                    PROJECT                                   |
 \____________________________________________________________________________/

________________________________________________________________________________
PART 1: TERRAIN GENERATION
¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯
  1. Height map Generation

    Create an FBO with a texture attached to it with 1 component, create and bind a shader program for rendering to that FBO. (Chris, Rehan)

    Create a Perlin noise function in the fragment shader. (Chris)

    !!!!! Render height map before the main render loop !!!! ce n'est pas fait mais n'a pas besoin d'être fait si c'est une carte infinie.

    Implement fractal Brownian motion (fBm). (Robin)

  2. Displaying the Terrain

    We took the quad from the homework 2 (trackball) because it contains a divided quad into multiple points. We simply renamed the quad into terrain. (Rehan)

    Diffuse shading of the terrain in the fragment shader. (Chris, Robin)

    Use simple schema to color the terrain according to height. (Robin)

  3. Advanced

    The keys: qweasd + arrows let the possibility to move in the virutal world. (Rehan)
    - w: move forward
    - s: move backward
    - a: the user is fixed, the point observed is moving to the left
    - d: the user is fixed, the point observed is moving to the right
    - q: the user and the point observed move to the left
    - e: the user and the point observed move to the right
    - left arrow: move like if there were a trackball
    - right arrow: move like if there were a trackball
    - up arrow: look up
    - down arrow: look down

  WORK TIMES [%]
    rehan :
    robin :
    chris :
