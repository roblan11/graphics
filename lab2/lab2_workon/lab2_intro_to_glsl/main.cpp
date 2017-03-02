// glew must be before glfw
#include <GL/glew.h>
#include <GLFW/glfw3.h>

// contains helper functions such as shader compiler
#include "icg_helper.h"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#define PI 3.14159265

// vertex position of the triangle
const GLfloat vpoint[] = {-1.0f, -1.0f, 0.0f,
                          1.0f, -1.0f, 0.0f,
                          0.0f,  1.0f, 0.0f,};

void Init() {
    // sets background color
    glClearColor(0.937, 0.937, 0.937 /*gray*/, 1.0 /*solid*/);

    // compile the shaders
    GLuint program_id = icg_helper::LoadShaders("triangle_vshader.glsl",
                                                "triangle_fshader.glsl");
    if(!program_id) {
        exit(EXIT_FAILURE);
    }
    glUseProgram(program_id);

    // set uniform values
    glm::mat4 ROTATE = glm::mat4(1);
    ROTATE[0][0] = cos(PI/4);
    ROTATE[0][1] = sin(PI/4);
    ROTATE[1][0] = -sin(PI/4);
    ROTATE[1][1] = cos(PI/4);
    GLuint ROTATE_id = glGetUniformLocation(program_id, "ROTATE");
    glUniformMatrix4fv(ROTATE_id, 1, GL_FALSE, glm::value_ptr(ROTATE));

    glm::mat4 SCALE = glm::mat4(1);
    SCALE[0][0] = 0.5;
    SCALE[1][1] = 0.7;
    GLuint SCALE_id = glGetUniformLocation(program_id, "SCALE");
    glUniformMatrix4fv(SCALE_id, 1, GL_FALSE, glm::value_ptr(SCALE));

    glm::mat4 TRANSLATE = glm::mat4(1);
    TRANSLATE[3][0] = 0.2;
    TRANSLATE[3][1] = 0.5;
    GLuint TRANSLATE_id = glGetUniformLocation(program_id, "TRANSLATE");
    glUniformMatrix4fv(TRANSLATE_id, 1, GL_FALSE, glm::value_ptr(TRANSLATE));

    // Vertex Array
    GLuint vertex_array_id;
    glGenVertexArrays(1, &vertex_array_id);
    glBindVertexArray(vertex_array_id);

    // Vertex Buffer
    GLuint vertex_buffer;
    glGenBuffers(1, &vertex_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vpoint), vpoint, GL_STATIC_DRAW);

    // position attribute, fetch Attribute ID for Vertex Positions
    GLuint position = glGetAttribLocation(program_id, "vpoint");
    glEnableVertexAttribArray(position); // enable it
    glVertexAttribPointer(position, 3, GL_FLOAT, DONT_NORMALIZE,
                          ZERO_STRIDE, ZERO_BUFFER_OFFSET);
}

void Display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glDrawArrays(GL_TRIANGLES, 0, 3);
}

void ErrorCallback(int error, const char* description) {
    fputs(description, stderr);
}

void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
}

int main(int argc, char *argv[]) {
    // GLFW Initialization
    if(!glfwInit()) {
        fprintf(stderr, "Failed to initialize GLFW\n");
        return EXIT_FAILURE;
    }

    glfwSetErrorCallback(ErrorCallback);

    // hint GLFW that we would like an OpenGL 3 context (at least)
    // http://www.glfw.org/faq.html#how-do-i-create-an-opengl-30-context
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // attempt to open the window: fails if required version unavailable
    // note some Intel GPUs do not support OpenGL 3.2
    // note update the driver of your graphic card
    GLFWwindow* window = glfwCreateWindow(512, 512, "intro to GLSL", NULL, NULL);
    if(!window) {
        glfwTerminate();
        return EXIT_FAILURE;
    }

    // makes the OpenGL context of window current on the calling thread
    glfwMakeContextCurrent(window);

    // set the callback for escape key
    glfwSetKeyCallback(window, KeyCallback);

    // GLEW Initialization (must have a context)
    // https://www.opengl.org/wiki/OpenGL_Loading_Library
    glewExperimental = GL_TRUE; // fixes glew error (see above link)
    if(glewInit() != GLEW_NO_ERROR) {
        fprintf( stderr, "Failed to initialize GLEW\n");
        return EXIT_FAILURE;
    }

    cout << "OpenGL" << glGetString(GL_VERSION) << endl;

    // initialize our OpenGL program
    Init();

    // render loop
    while(!glfwWindowShouldClose(window)) {
        Display();
        glfwSwapBuffers(window);
        glfwPollEvents();
    }


    // close OpenGL window and terminate GLFW
    glfwDestroyWindow(window);
    glfwTerminate();
    return EXIT_SUCCESS;
}
