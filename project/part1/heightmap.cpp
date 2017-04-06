//// glew must be before glfw
//#include <GL/glew.h>
//#include <GLFW/glfw3.h>

//// contains helper functions such as shader compiler
//#include "icg_helper.h"
//#include "glm/gtc/matrix_transform.hpp"

//#include "framebuffer.h"

//#include "terrain/terrain.h"
//#include "screenquad/screenquad.h"

////Cube cube;
//Terrain terrain2;

//int w_width = 800;
//int w_height = 600;

//FrameBuffer framebuffer2;
//ScreenQuad screenquad2;

//using namespace glm;

//mat4 projection_matrix;
//mat4 view_matrix;
//mat4 cube_model_matrix;
//mat4 terrain_model_matrix;

//void Init(GLFWwindow* window) {
//    glClearColor(1.0, 1.0, 1.0 /*white*/, 1.0 /*solid*/);
//    glEnable(GL_DEPTH_TEST);

//    //cube.Init();
//    terrain2.Init();

//    // create the model matrix (remember OpenGL is right handed)
//    // accumulated transformation
//    //cube_model_matrix = scale(IDENTITY_MATRIX, vec3(0.5));
//    //cube_model_matrix = translate(cube_model_matrix, vec3(0.0, 0.0, 0.6));
//    //terrain_model_matrix = IDENTITY_MATRIX;

//    // on retina/hidpi displays, pixels != screen coordinates
//    // this unsures that the framebuffer has the same size as the window
//    // (see http://www.glfw.org/docs/latest/window.html#window_fbsize)
//    glfwGetFramebufferSize(window, &w_width, &w_height);
//    GLuint framebuffer_texture_id = framebuffer2.Init(w_width, w_height);
//    screenquad2.Init(w_width, w_height, framebuffer_texture_id);
//}

//void Display() {
//    // render to framebuffer
//    framebuffer2.Bind();
//    {
//        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//        //cube.Draw(cube_model_matrix, view_matrix, projection_matrix);
//        terrain2.Draw(terrain_model_matrix, view_matrix, projection_matrix);
//    }
//    framebuffer2.Unbind();

//    // render to Window
//    glViewport(0, 0, w_width, w_height);
//    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//    screenquad2.Draw();
//}

//// gets called when the windows/framebuffer is resized.
//void ResizeCallback(GLFWwindow* window, int width, int height) {
//    w_width = width;
//    w_height = height;

//    //float ratio = w_width / (float) w_height;
//    //projection_matrix = perspective(45.0f, ratio, 0.1f, 10.0f);

//    //glViewport(0, 0, w_width, w_height);

//    // when the window is resized, the framebuffer and the screenquad
//    // should also be resized
//    framebuffer2.Cleanup();
//    framebuffer2.Init(w_width, w_height);
//    screenquad2.UpdateSize(w_width, w_height);
//}

//void ErrorCallback(int error, const char* description) {
//    fputs(description, stderr);
//}

//void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
//    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
//        glfwSetWindowShouldClose(window, GL_TRUE);
//    }
//}

//int main(int argc, char *argv[]) {
//    // GLFW Initialization
//    if(!glfwInit()) {
//        fprintf(stderr, "Failed to initialize GLFW\n");
//        return EXIT_FAILURE;
//    }

//    glfwSetErrorCallback(ErrorCallback);

//    // hint GLFW that we would like an OpenGL 3 context (at least)
//    // http://www.glfw.org/faq.html#how-do-i-create-an-opengl-30-context
//    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
//    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
//    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
//    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

//    // attempt to open the window: fails if required version unavailable
//    // note some Intel GPUs do not support OpenGL 3.2
//    // note update the driver of your graphic card
//    GLFWwindow* window = glfwCreateWindow(w_width, w_height,
//                                          "framebuffer", NULL, NULL);
//    if(!window) {
//        glfwTerminate();
//        return EXIT_FAILURE;
//    }

//    // makes the OpenGL context of window current on the calling thread
//    glfwMakeContextCurrent(window);

//    // set the callback for escape key
//    glfwSetKeyCallback(window, KeyCallback);

//    // set the framebuffer resize callback
//    glfwSetFramebufferSizeCallback(window, ResizeCallback);

//    // GLEW Initialization (must have a context)
//    // https://www.opengl.org/wiki/OpenGL_Loading_Library
//    glewExperimental = GL_TRUE; // fixes glew error (see above link)
//    if(glewInit() != GLEW_NO_ERROR) {
//        fprintf( stderr, "Failed to initialize GLEW\n");
//        return EXIT_FAILURE;
//    }

//    cout << "OpenGL" << glGetString(GL_VERSION) << endl;

//    // initialize our OpenGL program
//    Init(window);

//    // render loop
//    while(!glfwWindowShouldClose(window)){
//        Display();
//        glfwSwapBuffers(window);
//        glfwPollEvents();
//    }

//    // cleanup
//    terrain2.Cleanup();
//    //cube.Cleanup();
//    framebuffer2.Cleanup();
//    screenquad2.Cleanup();

//    // close OpenGL window and terminate GLFW
//    glfwDestroyWindow(window);
//    glfwTerminate();
//    return EXIT_SUCCESS;
//}

//int p[] = {171, 131, 182, 5, 239, 54, 114, 208, 130, 149, 209, 25, 161, 147, 251, 175, 22, 147, 127, 237, 200, 176, 166, 113, 11, 45, 132, 134, 216, 75, 99, 148, 67, 96, 46, 168, 15, 58, 232, 130, 87, 39, 128, 235, 75, 44, 234, 148, 71, 190, 25, 109, 242, 122, 70, 53, 185, 122, 170, 150, 201, 173, 249, 65, 65, 124, 88, 14, 34, 222, 29, 84, 216, 208, 201, 22, 67, 70, 72, 24, 190, 160, 151, 101, 16, 169, 60, 14, 219, 69, 48, 185, 243, 202, 89, 234, 72, 228, 44, 244, 114, 174, 102, 85, 152, 65, 247, 5, 254, 184, 206, 217, 122, 81, 91, 148, 111, 151, 177, 161, 24, 95, 217, 147, 219, 157, 205, 141, 49, 7, 243, 83, 89, 45, 22, 136, 208, 63, 171, 107, 217, 103, 153, 58, 10, 200, 76, 5, 36, 76, 134, 222, 213, 188, 132, 179, 53, 194, 216, 248, 84, 167, 212, 98, 227, 4, 1, 142, 64, 45, 239, 212, 24, 196, 53, 99, 233, 138, 157, 32, 69, 125, 232, 143, 163, 165, 142, 128, 59, 176, 147, 65, 55, 11, 79, 101, 1, 243, 183, 28, 133, 1, 213, 142, 188, 26, 106, 126, 99, 242, 171, 3, 88, 14, 203, 4, 200, 209, 133, 60, 98, 100, 191, 56, 10, 216, 11, 189, 135, 104, 86, 174, 214, 163, 70, 20, 133, 148, 185, 219, 55, 12, 98, 40, 176, 40, 161, 122, 129, 52, 86, 85, 33, 17, 173, 62};

//float fade(float t) {
//    return t * t * t * (t * (t * 6 - 15) + 10);
//}

//float lerp(float t, float a, float b) {
//    return a + t * (b - a);
//}

//float grad(int hash, float x, float y, float z) {
//      int h = hash & 15;                      // CONVERT LO 4 BITS OF HASH CODE
//      float u = h<8 ? x : y,                  // INTO 12 GRADIENT DIRECTIONS.
//             v = h<4 ? y : h==12||h==14 ? x : z;
//      return ((h&1) == 0 ? u : -u) + ((h&2) == 0 ? v : -v);
//}

//float noise(float x, float y, float z) {
//      int X = int(floor(x)) & 255,                  // FIND UNIT CUBE THAT
//          Y = int(floor(y)) & 255,                  // CONTAINS POINT.
//          Z = int(floor(z)) & 255;
//      x -= floor(x);                                // FIND RELATIVE X,Y,Z
//      y -= floor(y);                                // OF POINT IN CUBE.
//      z -= floor(z);
//      float u = fade(x),                                 // COMPUTE FADE CURVES
//            v = fade(y),                                 // FOR EACH OF X,Y,Z.
//            w = fade(z);
//      int A = p[X  ]+Y, AA = p[A]+Z, AB = p[A+1]+Z,      // HASH COORDINATES OF
//          B = p[X+1]+Y, BA = p[B]+Z, BB = p[B+1]+Z;      // THE 8 CUBE CORNERS,

//      return lerp(w, lerp(v, lerp(u, grad(p[AA  ], x  , y  , z   ),  // AND ADD
//                                     grad(p[BA  ], x-1, y  , z   )), // BLENDED
//                             lerp(u, grad(p[AB  ], x  , y-1, z   ),  // RESULTS
//                                     grad(p[BB  ], x-1, y-1, z   ))),// FROM  8
//                     lerp(v, lerp(u, grad(p[AA+1], x  , y  , z-1 ),  // CORNERS
//                                     grad(p[BA+1], x-1, y  , z-1 )), // OF CUBE
//                             lerp(u, grad(p[AB+1], x  , y-1, z-1 ),
//                                     grad(p[BB+1], x-1, y-1, z-1 ))));
//}
