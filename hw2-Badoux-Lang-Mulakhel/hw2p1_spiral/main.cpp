#include<iostream>

const GLfloat vpoint_buffer[] = {
 /*V1*/ -1.0f, -1.0f, 0.0f,
 /*V2*/  1.0f, -1.0f, 0.0f,
 /*V3*/  0.0f,  1.0f, 0.0f};

// intro_to_glsl/triangle_vshader.glsl
#version 330
in vec3 vpoint;
void main(){
    gl_Position = vec4(vpoint, 1.0);
}

// intro_to_glsl/triangle_fshader.glsl
#version 330 core
out vec3 color;
void main(){
    color = vec3(1.0, 0.0, 0.0);
}
