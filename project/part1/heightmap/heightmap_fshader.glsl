#version 330

in vec2 uv;

out vec3 color;

uniform sampler2D tex;
uniform float tex_width;
uniform float tex_height;
uniform float position_looking_at_x;
uniform float position_looking_at_y;
uniform int[256] p;

// Properties
uniform int octaves;
uniform float lacunarity;
uniform float gain;
uniform float amplitude;
uniform float exponent;
uniform float heightscale;
uniform float offset;
uniform float scale;

float fade(float t) {
    return t * t * t * (t * (t * 6 - 15) + 10);
}

float lerp(float t, float a, float b) {
    return a + t * (b - a);
}

float grad(int hash, float x, float y, float z) {
    // CONVERT LO 4 BITS OF HASH CODE INTO 12 GRADIENT DIRECTIONS.
    int h = hash & 15;
    float u = h<8 ? x : y,
          v = h<4 ? y : h==12||h==14 ? x : z;
    return ((h&1) == 0 ? u : -u) + ((h&2) == 0 ? v : -v);
}

float noise(float x, float y, float z) {
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
    return lerp(w, lerp(v, lerp(u, grad(p[AA  ], x  , y  , z   ),
                                   grad(p[BA  ], x-1, y  , z   )),
                           lerp(u, grad(p[AB  ], x  , y-1, z   ),
                                   grad(p[BB  ], x-1, y-1, z   ))),
                   lerp(v, lerp(u, grad(p[AA+1], x  , y  , z-1 ),
                                   grad(p[BA+1], x-1, y  , z-1 )),
                           lerp(u, grad(p[AB+1], x  , y-1, z-1 ),
                                   grad(p[BB+1], x-1, y-1, z-1 ))));
}

float fBm(vec2 xy) {
    // source: https://thebookofshaders.com/13/

    float amp = amplitude;
    float z = 0.f;

    // Loop of octaves
    for (int i = 0; i < octaves; i++) {
        float n = noise(xy.x, xy.y, 0.f);
        n = 1 - abs(n);
        n = pow(n, exponent);
        z += amp * (n*heightscale - offset);

        xy *= lacunarity;
        amp *= gain;
    }
    return z;
}

void main() {

    vec2 xy = uv.xy;
    xy.x += position_looking_at_x;
    xy.y += position_looking_at_y;

    float red = fBm(abs(scale*xy));

    if(red < 0){
        color =  vec3(0.0, 0.0, 0.0);
    } else {
        color =  vec3(red, 0.0, 0.0);
    }
    //color = vec3(noise(uv.x*7, uv.y*7, 0.0), 0.0, 0.0);
}
