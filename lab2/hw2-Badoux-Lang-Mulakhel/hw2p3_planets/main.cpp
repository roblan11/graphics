// glew must be before glfw
#include <GL/glew.h>
#include <GLFW/glfw3.h>

// contains helper functions such as shader compiler
#include "icg_helper.h"
#include "quad/quad.h"

#define ELLIPSE_A 0.8
#define ELLIPSE_B 0.6
#define ELLIPSE_C 0.2
#define MOON_ORBIT_RADIUS 0.2

#define SCALE_MOON 0.04
#define SCALE_EARTH 0.08
#define SCALE_SUN 0.2

// DEFINE SPEEDUP HERE =========================================================
// 1.0 = normal speed  // > 1.0 = faster // < 1.0 = slower
#define SPEEDUP 1.0

Quad moon;
Quad earth;
Quad sun;

void Init() {
    // sets background color
    glClearColor(1.0,1.0,1.0 /*white*/, 1.0 /*solid*/);
    moon.Init("moon.tga");
    earth.Init("earth.tga");
    sun.Init("sun.tga");
}

glm::mat4 getTranlationMatrixOfTheEarth(float time) {
    return glm::translate(glm::mat4(1), glm::vec3(ELLIPSE_A * cos(time/3), -ELLIPSE_B * sin(time/3), 0));
}

glm::mat4 computeMoonModel(float time) {
    glm::mat4 S = glm::scale(glm::mat4(1), glm::vec3(SCALE_MOON));
    glm::mat4 R = glm::rotate(glm::mat4(1), time, glm::vec3(0, 0, 1));
    glm::mat4 orbitRadius = glm::translate(glm::mat4(1), glm::vec3(MOON_ORBIT_RADIUS*cos(time), MOON_ORBIT_RADIUS*sin(time), 0));

    return orbitRadius * getTranlationMatrixOfTheEarth(time) * S * R;
}

glm::mat4 computeEarthModel(float time) {
    glm::mat4 T = getTranlationMatrixOfTheEarth(time);
    glm::mat4 S = glm::scale(glm::mat4(1), glm::vec3(SCALE_EARTH));
    glm::mat4 R = glm::rotate(glm::mat4(1), 2*time, glm::vec3(0, 0, 1));

    return T * S * R;
}

glm::mat4 computeSunModel(float time) {
    glm::mat4 T = glm::translate(glm::mat4(1), glm::vec3(ELLIPSE_C, 0, 0));
    glm::mat4 S = glm::scale(glm::mat4(1), glm::vec3(SCALE_SUN));
    glm::mat4 R = glm::rotate(glm::mat4(1), time/2, glm::vec3(0, 0, 1));

    return T * S * R;
}

void Display() {
    glClear(GL_COLOR_BUFFER_BIT);
    float time_s = SPEEDUP * glfwGetTime();

    // compute the transformation matrices
    earth.Draw(computeEarthModel(time_s));
    sun.Draw(computeSunModel(time_s));
    moon.Draw(computeMoonModel(time_s));
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
    GLFWwindow* window = glfwCreateWindow(512, 512, "planets", NULL, NULL);
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

    earth.Cleanup();
    moon.Cleanup();
    sun.Cleanup();

    // close OpenGL window and terminate GLFW
    glfwDestroyWindow(window);
    glfwTerminate();
    return EXIT_SUCCESS;
}
