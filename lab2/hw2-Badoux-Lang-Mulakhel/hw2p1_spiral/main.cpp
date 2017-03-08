// glew must be before glfw
#include <GL/glew.h>
#include <GLFW/glfw3.h>

// contains helper functions such as shader compiler
#include "icg_helper.h"
#include <glm/gtc/matrix_transform.hpp>
#include "triangle/triangle.h"

#define FERMAT_SCALE 0.02
#define SPIRAL_SCALE 0.001
#define PI 3.14159

Triangle triangle;

enum class MODE {SPIRAL, FERMAT};
// CHANGE MODE HERE ============================================================
MODE mode = MODE::SPIRAL;

void Init() {
    // sets background color
    glClearColor(0.937, 0.937, 0.937 /*gray*/, 1.0 /*solid*/);

    triangle.Init();
}

void Display() {
    glClear(GL_COLOR_BUFFER_BIT);
    if (mode == MODE::FERMAT) {
        glm::mat4 R = glm::mat4(1);
        glm::mat4 S = glm::scale(glm::mat4(1), glm::vec3(FERMAT_SCALE));
        glm::mat4 T = glm::mat4(1);
        glm::mat4 model;

        float alpha;
        float scale;

        for (size_t n = 0; n < 300; n++) {
            // rotation
            alpha = n * (137.508 / 180 * PI);
            R = glm::rotate(glm::mat4(1), alpha, glm::vec3(0, 0, 1));

            // translation
            T[3][0] = sqrt(n) * 0.04;

            model = R * T * S;
            triangle.Draw(model);
        }
    } else {
        // spiral
        glm::mat4 R = glm::mat4(1);
        glm::mat4 S = glm::mat4(1);
        glm::mat4 T = glm::mat4(1);
        glm::mat4 model;

        float alpha;
        float scale;

        for (size_t n = 0; n < 60; n++) {
            // rotation
            alpha = n * PI/10;
            R = glm::rotate(glm::mat4(1), alpha, glm::vec3(0, 0, 1));

            // scale
            S = glm::scale(glm::mat4(1), glm::vec3(n * SPIRAL_SCALE));

            // translation
            T[3][0] = n * 0.15;

            model = R * S * T;
            triangle.Draw(model);
        }
    }
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
    GLFWwindow* window = glfwCreateWindow(512, 512, "spiral", NULL, NULL);
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

    triangle.Cleanup();

    // close OpenGL window and terminate GLFW
    glfwDestroyWindow(window);
    glfwTerminate();
    return EXIT_SUCCESS;
}
