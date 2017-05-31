#version 330 core
uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelMatrix;
uniform vec3 cameraPosition;
in vec3 vpoint;
in vec2 vtexcoord;
out vec2 uv;
out vec3 toCameraVector;

const float tiling = 3.0;

void main() {
    vec4 worldPosition = modelMatrix * vec4(vpoint, 1.0);
    gl_Position = projectionMatrix * viewMatrix * worldPosition;
    uv = vec2(vtexcoord.x / 2.0 + 0.5, vtexcoord.y / 2.0 + 0.5) * tiling;
    toCameraVector = cameraPosition - worldPosition.xyz;
}
