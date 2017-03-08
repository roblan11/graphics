#include <iostream>

// glew must be before glfw
#include <GL/glew.h>
#include <GLFW/glfw3.h>

// contains helper functions such as shader compiler
#include "icg_helper.h"

#include "quad/quad.h"

#define BALL_RADIUS 0.04
#define PAD_WIDTH 0.1
#define PAD_HEIGHT 0.02

Quad ball;
Quad spaceship;
float time0;
float ballX0;
float ballY0;
float ballSpeedX;
float ballSpeedY;
float padX;

void Init() {
    // sets background color
    glClearColor(0.937, 0.937, 0.937 /*gray*/, 1.0 /*solid*/);

    ball.Init();
    spaceship.Init();
    time0 = 0;
    ballX0 = 0;
    ballY0 = 0;
    ballSpeedX = 0.3;
    ballSpeedY = 0.5;
    padX = 0;
}

glm::mat4 computeBallModel(float t) { // current time
    float deltaT = t - time0;
    float posX = ballX0 + ballSpeedX * (deltaT);
    float posY = ballY0 + ballSpeedY * (deltaT);
    bool overH = ((posX + BALL_RADIUS > 1 && ballSpeedX > 0) || (posX - BALL_RADIUS < -1 && ballSpeedX < 0));
    bool overV = ((posY + BALL_RADIUS > 1 && ballSpeedY > 0) || (posY - BALL_RADIUS - PAD_HEIGHT < -1 && ballSpeedY < 0));
    if (overH || overV) {
        time0 = t;
        ballX0 = posX;
        ballY0 = posY;
        if (overH) {
            ballSpeedX *= -1;
        }
        if (overV) {
            ballSpeedY *= -1;
            if (posY < 0 && abs(padX - posX) > PAD_WIDTH / 2) {
                cout << "NOOOOOOOOOOOOOO" << endl;
            }
        }
    }

    glm::mat4 T = glm::translate(glm::mat4(1), glm::vec3(posX, posY, 0));
    glm::mat4 S = glm::scale(glm::mat4(1), glm::vec3(BALL_RADIUS));
    return T * S;
}

void movePad(int direction) {
    if (abs(padX) < 1) {
        padX += direction * 0.1;
    }
}

glm::mat4 computePadModel() {
    glm::mat4 T = glm::translate(glm::mat4(1), glm::vec3(padX, -0.98, 0));
    glm::mat4 S = glm::scale(glm::mat4(1), glm::vec3(PAD_WIDTH, PAD_HEIGHT, 0));
    return T * S;
}

void Display() {
    glClear(GL_COLOR_BUFFER_BIT);
    ball.Draw(computeBallModel(glfwGetTime()));
    spaceship.Draw(computePadModel());
}

void ErrorCallback(int error, const char* description) {
    fputs(description, stderr);
}

void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GL_TRUE);
    } else if (key == GLFW_KEY_LEFT && action == GLFW_PRESS) {
        movePad(-1);
    } else if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS) {
        movePad(1);
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
