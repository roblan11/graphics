#version 330

in vec2 position;

out vec2 uv;

uniform mat4 MVP;
uniform float time;

void main() {
    uv = (position + vec2(1.0, 1.0)) * 0.5;

    // convert the 2D position into 3D positions that all lay in a horizontal
    // plane.
    // TODO 6: animate the height of the grid points as a sine function of the
    // 'time' and the position ('uv') within the grid.
    const float amplitude = 0.1;
    const float waveSpeedX = 0.75;
    const float waveSpeedY = 0.5;
    const float omegaX = 2 * 3.14 * 2.2;
    const float omegaY = 2 * 3.14 * 1.9;
    float tmpX = uv.x * omegaX + waveSpeedX * time;
    float tmpY = uv.y * omegaY + waveSpeedY * time;
    float height = amplitude * sin(tmpX + tmpY);
    vec3 pos_3d = vec3(position.x, height, -position.y);
    
    gl_Position = MVP * vec4(pos_3d, 1.0);
}
