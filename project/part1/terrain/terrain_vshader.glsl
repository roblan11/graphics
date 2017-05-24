#version 330

in vec2 position;

out vec4 vpoint_mv;
out vec3 light_dir, view_dir;
out vec2 uv;
out float height;

uniform mat4 MVP;
uniform float time;
uniform bool water;
uniform mat4 MV;
uniform vec3 light_pos;

uniform sampler2D tex;

uniform vec4 clippingPlane;

void main() {
    uv = (position + vec2(1.0, 1.0)) * 0.5;

    height = texture(tex,uv).x;
    vec3 pos_3d = vec3(position.x, position.y, height);

    vpoint_mv = MV * vec4(pos_3d, 1.0);
    gl_ClipDistance[0] = dot(vec4(pos_3d, 1.0), clippingPlane);
    //gl_ClipDistance[0] = dot(vpoint_mv, clippingPlane);

    gl_Position = MVP * vec4(pos_3d, 1.0);

    light_dir = light_pos - vpoint_mv.xyz;
    view_dir = -vpoint_mv.xyz;
}

