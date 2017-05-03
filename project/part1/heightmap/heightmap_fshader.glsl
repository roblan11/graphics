#version 330

in vec2 uv;

out vec3 color;

uniform sampler2D tex;
uniform float tex_width;
uniform float tex_height;
uniform float position_looking_at_x;
uniform float position_looking_at_y;
uniform int[256] p;

float fade(float t) {
    return t * t * t * (t * (t * 6 - 15) + 10);
}

float lerp(float t, float a, float b) {
    return a + t * (b - a);
}

/*float grad_3d(int hash, float x, float y, float z) {
    // CONVERT LO 4 BITS OF HASH CODE INTO 12 GRADIENT DIRECTIONS.
    int h = hash & 15;
    float u = h<8 ? x : y,
          v = h<4 ? y : h==12||h==14 ? x : z;
    return ((h&1) == 0 ? u : -u) + ((h&2) == 0 ? v : -v);
}

float noise_3d(float x, float y, float z) {
    // FIND UNIT CUBE THAT CONTAINS POINT.
    int X = int(floor(x)) & 255,
        Y = int(floor(y)) & 255,
        Z = int(floor(z)) & 255;
    // FIND RELATIVE X,Y,Z OF POINT IN CUBE.
    x -= floor(x);
    y -= floor(y);
    z -= floor(z);
    // COMPUTE FADE CURVES FOR EACH OF X,Y,Z.
    float u = fade(x),
          v = fade(y),
          w = fade(z);
    // HASH COORDINATES OF THE 8 CUBE CORNERS,
    int A = p[X  ]+Y, AA = p[A]+Z, AB = p[A+1]+Z,
        B = p[X+1]+Y, BA = p[B]+Z, BB = p[B+1]+Z;
    // AND ADD BLENDED RESULTS FROM  8 CORNERS OF CUBE
    return lerp(w, lerp(v, lerp(u, grad_3d(p[AA  ], x  , y  , z   ),
                                   grad_3d(p[BA  ], x-1, y  , z   )),
                           lerp(u, grad_3d(p[AB  ], x  , y-1, z   ),
                                   grad_3d(p[BB  ], x-1, y-1, z   ))),
                   lerp(v, lerp(u, grad_3d(p[AA+1], x  , y  , z-1 ),
                                   grad_3d(p[BA+1], x-1, y  , z-1 )),
                           lerp(u, grad_3d(p[AB+1], x  , y-1, z-1 ),
                                   grad_3d(p[BB+1], x-1, y-1, z-1 ))));
}*/

float grad_2d(int hash, float x, float y) {
    switch(hash & 0x3)
    {
        case 0x0: return  x + y;
        case 0x1: return -x + y;
        case 0x2: return  x - y;
        case 0x3: return -x - y;
        default: return 0.f; // never happens
    }
}

float noise(float x, float y, float z) {
      int X = int(floor(x)) & 255,                  // FIND UNIT CUBE THAT
          Y = int(floor(y)) & 255,                  // CONTAINS POINT.
          Z = int(floor(z)) & 255;
      x -= floor(x);                                // FIND RELATIVE X,Y,Z
      y -= floor(y);                                // OF POINT IN CUBE.
      z -= floor(z);
      float u = fade(x),                                 // COMPUTE FADE CURVES
            v = fade(y),                                 // FOR EACH OF X,Y,Z.
            w = fade(z);
      int A = p[X  ]+Y, AA = p[A]+Z, AB = p[A+1]+Z,      // HASH COORDINATES OF
          B = p[X+1]+Y, BA = p[B]+Z, BB = p[B+1]+Z;      // THE 8 CUBE CORNERS,

      return lerp(w, lerp(v, lerp(u, grad(p[AA  ], x  , y  , z   ),  // AND ADD
                                     grad(p[BA  ], x-1, y  , z   )), // BLENDED
                             lerp(u, grad(p[AB  ], x  , y-1, z   ),  // RESULTS
                                     grad(p[BB  ], x-1, y-1, z   ))),// FROM  8
                     lerp(v, lerp(u, grad(p[AA+1], x  , y  , z-1 ),  // CORNERS
                                     grad(p[BA+1], x-1, y  , z-1 )), // OF CUBE
                             lerp(u, grad(p[AB+1], x  , y-1, z-1 ),
                                     grad(p[BB+1], x-1, y-1, z-1 ))));
}

float fBm(vec2 xy) {
  // source: https://thebookofshaders.com/13/

  // Properties
  const int octaves = 5;
  float lacunarity = 2.f;
  float gain = 0.35;
  //
  // Initial values
  float amplitude = 0.7;

  float z = 0.f;
  //
  // Loop of octaves
  for (int i = 0; i < octaves; i++) {
    float n = noise(xy.x, xy.y, 0.f);
    n = 1 - abs(n);     // create creases
    n = pow(n, 0.8);      // sharpen creases
    z += amplitude * (n*1.3 - 0.9);

    xy *= lacunarity;
    amplitude *= gain;
  }
  return z;
}

void main() {

    vec2 xy = uv.xy;
    xy.x += position_looking_at_x;
    xy.y += position_looking_at_y;

    float red = fBm(2*xy + vec2(17.f, 6.f));

    color =  vec3(red, 0.0, 0.0);
    //color = vec3(noise(uv.x*7, uv.y*7, 0.0), 0.0, 0.0);
}
