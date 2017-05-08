// glew must be before glfw
#include <GL/glew.h>
#include <GLFW/glfw3.h>

// contains helper functions such as shader compiler
#include "icg_helper.h"
#include "glm/gtc/matrix_transform.hpp"

#include "camera/camera.h"

#include "framebuffer.h"
#include "heightmap/heightmap.h"

#include "skybox/skybox.h"
#include "terrain/terrain.h"
#include "water/water.h"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw_gl3.h"

//Cube cube;
Terrain terrain;

int window_width = 800;
int window_height = 600;

Camera camera;
FrameBuffer framebuffer;
FrameBuffer reflectionBuffer;
HeightMap heightmap;
Skybox skybox;
Water water;

using namespace glm;

mat4 projection_matrix;
//mat4 cube_model_matrix;
mat4 terrain_model_matrix;
mat4 skybox_model_matrix;
mat4 water_model_matrix;

void Init(GLFWwindow* window) {
    glClearColor(1.0, 1.0, 1.0 /*white*/, 1.0 /*solid*/);
    glEnable(GL_DEPTH_TEST);

    // setup view and projection matrices
    float ratio = window_width / (float) window_height;
    projection_matrix = perspective(45.0f, ratio, 0.1f, 10.0f);

    terrain_model_matrix = scale(IDENTITY_MATRIX, vec3(5.0f));
    water_model_matrix = scale(IDENTITY_MATRIX, vec3(5.0f));
    water_model_matrix[3][2] = 0.05f;
    skybox_model_matrix = rotate(scale(IDENTITY_MATRIX, vec3(9.0f)), 1.57f, vec3(1,0,0));

    glfwGetFramebufferSize(window, &window_width, &window_height);
    GLuint framebuffer_texture_id = framebuffer.Init(window_width, window_height);
    GLuint reflection_texture_id = reflectionBuffer.Init(window_width, window_height);
    camera.Init(vec3(-4, -1.0f, 4), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, 1.0f));
    heightmap.Init(window_width, window_height, framebuffer_texture_id);
    skybox.Init();
    terrain.Init(framebuffer_texture_id);
    water.Init(reflection_texture_id);
}

// transforms glfw screen coordinates into normalized OpenGL coordinates.
vec2 TransformScreenCoords(GLFWwindow* window, int x, int y) {
    // the framebuffer and the window doesn't necessarily have the same size
    // i.e. hidpi screens. so we need to get the correct one
    int width;
    int height;
    glfwGetWindowSize(window, &width, &height);
    return vec2(2.0f * (float)x / width - 1.0f,
                1.0f - 2.0f * (float)y / height);
}

void Display() {
    mat4 view_matrix = camera.ViewMatrix(false);
    // render to framebuffer
    framebuffer.Bind();
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        heightmap.Draw(camera.getPositionLookingAtX(), camera.getPositionLookingAtY());
    }
    framebuffer.Unbind();

    reflectionBuffer.Bind();
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        mat4 view_mirrored = camera.ViewMatrix(true);
        terrain.Draw(terrain_model_matrix, view_mirrored, projection_matrix);
        skybox.Draw(skybox_model_matrix, view_mirrored, projection_matrix);
    }
    reflectionBuffer.Unbind();

    // render to Window
    glViewport(0, 0, window_width, window_height);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    terrain.Draw(terrain_model_matrix, view_matrix, projection_matrix);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    water.Draw(water_model_matrix, view_matrix, projection_matrix);
    glDisable(GL_BLEND);
    skybox.Draw(skybox_model_matrix, view_matrix, projection_matrix);
}

// gets called when the windows/framebuffer is resized.
void ResizeCallback(GLFWwindow* window, int width, int height) {
    window_width = width;
    window_height = height;

    float ratio = window_width / (float) window_height;
    projection_matrix = perspective(45.0f, ratio, 0.1f, 10.0f);

    glViewport(0, 0, window_width, window_height);

    // when the window is resized, the framebuffer and the screenquad
    // should also be resized
    framebuffer.Cleanup();
    framebuffer.Init(window_width, window_height);
}

void ErrorCallback(int error, const char* description) {
    fputs(description, stderr);
}

void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GL_TRUE);
    } else if (action == GLFW_PRESS) {
        if (key == GLFW_KEY_W) {
            camera.MoveForward();
        } else if (key == GLFW_KEY_S) {
            camera.MoveBackward();
        } else if (key == GLFW_KEY_A) {
            camera.LookOnTheLeft();
        } else if (key == GLFW_KEY_D) {
            camera.LookOnTheRight();
        }

        // move model_matrix
        terrain_model_matrix[3][0] = camera.getPositionLookingAtX();
        terrain_model_matrix[3][1] = camera.getPositionLookingAtY();
        water_model_matrix[3][0] = camera.getPositionLookingAtX();
        water_model_matrix[3][1] = camera.getPositionLookingAtY();
        skybox_model_matrix[3][0] = camera.getPositionLookingAtX();
        skybox_model_matrix[3][1] = camera.getPositionLookingAtY();
    }
}

void GUI(GLFWwindow* window) {
    static int octaves = 5;
    static float lacunarity = 2.0f;
    static float gain = 0.35;
    static float amplitude = 0.7;
    static float exponent = 0.8;
    static float heightscale = 1.3;
    static float offset = 0.9;
    static float scale = 2.f;

    ImGui_ImplGlfwGL3_NewFrame();

    {
        ImGui::Begin("heightmap options");

        ImGui::SliderInt("# octaves", &octaves, 1, 10);
        ImGui::SliderFloat("lacunarity", &lacunarity, 0.1f, 5.0f);
        ImGui::SliderFloat("gain", &gain, 0.01f, 1.0f);
        ImGui::SliderFloat("amplitude", &amplitude, 0.01f, 1.0f);
        ImGui::SliderFloat("exponent", &exponent, 0.01f, 3.0f);
        ImGui::SliderFloat("scale the height", &heightscale, 0.1f, 3.0f);
        ImGui::SliderFloat("negative offset", &offset, 0.f, 2.f);
        ImGui::SliderFloat("terrain scale", &scale, 0.1f, 3.0f);
        ImGui::End();

        heightmap.Update(octaves, lacunarity, gain, amplitude, exponent, heightscale, offset, scale);
    }

    static float mix_uw_sand = 0.005;
    static float mix_sand_grass = 0.015;
    static float lvl_grass = 0.12;
    static float mix_grass_rock = 0.18;
    static float lvl_rock = 0.3;
    static float mix_rock_snow = 0.35;
    static float lvl_snow = 0.4;

    static float scale_uw = 2.f;
    static float scale_sand = 10.f;
    static float scale_grass = 10.f;
    static float scale_rock = 5.f;
    static float scale_snow = 5.f;

    {
        ImGui::Begin("terrain options");
        ImGui::Text("heights");
        ImGui::SliderFloat("waterline mix", &mix_uw_sand, 0.f, mix_sand_grass);
        ImGui::SliderFloat("sand-grass mix", &mix_sand_grass, mix_uw_sand, lvl_grass);
        ImGui::SliderFloat("grass lvl", &lvl_grass, mix_sand_grass, mix_grass_rock);
        ImGui::SliderFloat("grass-rock mix", &mix_grass_rock, lvl_grass, lvl_rock);
        ImGui::SliderFloat("rock lvl", &lvl_rock, mix_grass_rock, mix_rock_snow);
        ImGui::SliderFloat("rock-snow mix", &mix_rock_snow, lvl_rock, lvl_snow);
        ImGui::SliderFloat("snow lvl", &lvl_snow, mix_rock_snow, 1.f);

        ImGui::Text("texture scale");
        ImGui::SliderFloat("uw tex", &scale_uw, 1.f, 20.f);
        ImGui::SliderFloat("sand tex", &scale_sand, 1.f, 20.f);
        ImGui::SliderFloat("grass tex", &scale_grass, 1.f, 20.f);
        ImGui::SliderFloat("rock tex", &scale_rock, 1.f, 20.f);
        ImGui::SliderFloat("snow tex", &scale_snow, 1.f, 20.f);

        ImGui::End();

        terrain.Update(mix_uw_sand, mix_sand_grass, lvl_grass, mix_grass_rock, lvl_rock, mix_rock_snow, lvl_snow, scale_uw, scale_sand, scale_grass, scale_rock, scale_snow);

    }
    {
        ImGui::Begin("general information");
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        ImGui::End();
    }

    ImGui::Render();
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
    GLFWwindow* window = glfwCreateWindow(window_width, window_height,
                                          "framebuffer", NULL, NULL);
    if(!window) {
        glfwTerminate();
        return EXIT_FAILURE;
    }

    // makes the OpenGL context of window current on the calling thread
    glfwMakeContextCurrent(window);

    // set the callback for escape key
    glfwSetKeyCallback(window, KeyCallback);

    // set the framebuffer resize callback
    glfwSetFramebufferSizeCallback(window, ResizeCallback);

    // GLEW Initialization (must have a context)
    // https://www.opengl.org/wiki/OpenGL_Loading_Library
    glewExperimental = GL_TRUE; // fixes glew error (see above link)
    if(glewInit() != GLEW_NO_ERROR) {
        fprintf( stderr, "Failed to initialize GLEW\n");
        return EXIT_FAILURE;
    }

    cout << "OpenGL" << glGetString(GL_VERSION) << endl;

    // initialize our OpenGL program
    Init(window);
    ImGui_ImplGlfwGL3_Init(window, false);

    // render loop
    while(!glfwWindowShouldClose(window)){
        Display();

        GUI(window);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    ImGui_ImplGlfwGL3_Shutdown();

    // cleanup
    terrain.Cleanup();
    framebuffer.Cleanup();
    heightmap.Cleanup();

    // close OpenGL window and terminate GLFW
    glfwDestroyWindow(window);
    glfwTerminate();
    return EXIT_SUCCESS;
}
