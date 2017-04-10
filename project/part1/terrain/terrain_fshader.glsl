#version 330

in vec2 uv;

out vec3 color;

uniform sampler2D tex;
uniform mat4 MV;

void main() {
    float temp = texture(tex, uv).x;
    if(temp < 0.001){
        color = vec3(0, 0.6, 0.9);
    } else if (temp < 0.02){
        color = vec3(0.6, 0.7, 0.0);
    } else if (temp < 0.07){
        color = mix(vec3(0.6, 0.7, 0.0), vec3(0.2, 0.7, 0.1), (temp-0.02)/0.07);
    } else if (temp < 0.2){
        color = vec3(0.2, 0.7, 0.1);
    } else if (temp < 0.4){
        color = mix(vec3(0.2, 0.7, 0.1), vec3(0.9, 0.9, 0.9), (temp-0.2)/0.2);
    } else {
        color = vec3(0.8, 0.8, 0.8);
    }

    float dx = (texture(tex, vec2(uv.x+1, uv.y)).x - texture(tex, vec2(uv.x-1, uv.y)).x);
    float dy = (texture(tex, vec2(uv.x, uv.y + 1)).x - texture(tex, vec2(uv.x, uv.y - 1)).x);
    vec3 direction = vec3(-dx,-dy,1.0);
    vec3 vnormal = normalize(direction);

    vec3 kd = vec3(0.9f, 0.5f, 0.5f);
    vec3 Ld = vec3(1.0f, 1.0f, 1.0f);
    vec3 light_pos = vec3(0.0f, 0.0f, 2.0f);
    vec3 light_dir = light_pos - temp;

    vec3 normal_mv = mat3(transpose(inverse(MV))) * vnormal;

    vec3 n = normalize(normal_mv);
    vec3 l = normalize(light_dir);
    float lambert = dot(n, l);

    if(lambert > 0.0) {
        color += Ld * kd * lambert;
    }
}
