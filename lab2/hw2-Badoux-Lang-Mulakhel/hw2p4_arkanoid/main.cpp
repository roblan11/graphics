// glew must be before glfw
#include <GL/glew.h>
#include <GLFW/glfw3.h>

// contains helper functions such as shader compiler
#include "icg_helper.h"

#include "quad/quad.h"

#define SCALE_BALL 0.04
#define SCALE_SPACESHIP1 0.4
#define SCALE_SPACESHIP2 0.04

Quad ball;
Quad spaceship;

void Init() {
    // sets background color
    glClearColor(0.937, 0.937, 0.937 /*gray*/, 1.0 /*solid*/);

    ball.Init();
    spaceship.Init();
}

glm::mat4 T = glm::mat4(1);

glm::mat4 computeBallModel(float v1, float v2) {
    if(T[3][0] < -0.5 || T[3][0] > 0.5){
        T[3][0] -= v1;
        T[3][1] -= v2;
        cout << T[3][0];
    }
    else{
        T[3][0] += v1;
        T[3][1] += v2;
        cout << T[3][0];
    }

    glm::mat4 S = glm::mat4(1);
    S[0][0] = SCALE_BALL;
    S[1][1] = SCALE_BALL;

    return T * S;
}

glm::mat4 computeSpaceshipModel(float v1) {
    glm::mat4 T = glm::mat4(1);
    T[3][0] = -v1;
    T[3][1] = -0.5;

    glm::mat4 S = glm::mat4(1);
    S[0][0] = SCALE_SPACESHIP1;
    S[1][1] = SCALE_SPACESHIP2;

    return T * S;
}

void Display() {
    glClear(GL_COLOR_BUFFER_BIT);
    ball.Draw(computeBallModel(0.01,0.01));
    spaceship.Draw(computeSpaceshipModel(0));
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
    GLFWwindow* window = glfwCreateWindow(512, 512, "checkerboard", NULL, NULL);
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

    ball.Cleanup();
    spaceship.Cleanup();

    // close OpenGL window and terminate GLFW
    glfwDestroyWindow(window);
    glfwTerminate();
    return EXIT_SUCCESS;
}
