#pragma once
#include "icg_helper.h"
#include "glm/gtc/type_ptr.hpp"

float WAVE_SPEED = 0.0005;

class Water {

    private:
        GLuint vertex_array_id_;        // vertex array object
        GLuint program_id_;             // GLSL shader program ID
        GLuint vertex_buffer_object_;   // memory buffer
        GLuint texture_dudv_id_;             // texture ID
        GLuint texture_mirror_id_;      // texture mirror ID
        GLuint move_factor_id_;
        float moveFactor;

    public:
        void Init(GLuint tex_mirror) {
            // compile the shaders
            program_id_ = icg_helper::LoadShaders("water_vshader.glsl", "water_fshader.glsl");
            if(!program_id_) {
                exit(EXIT_FAILURE);
            }

            glUseProgram(program_id_);

            moveFactor = 0.0f;

            // vertex one vertex Array
            glGenVertexArrays(1, &vertex_array_id_);
            glBindVertexArray(vertex_array_id_);

            // vertex coordinates
            {
                const GLfloat vertex_point[] = { /*V1*/ -1.0f, -1.0f, 0.0f,
                                                 /*V2*/ +1.0f, -1.0f, 0.0f,
                                                 /*V3*/ -1.0f, +1.0f, 0.0f,
                                                 /*V4*/ +1.0f, +1.0f, 0.0f};
                // buffer
                glGenBuffers(1, &vertex_buffer_object_);
                glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_object_);
                glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_point),
                             vertex_point, GL_STATIC_DRAW);

                // attribute
                GLuint vertex_point_id = glGetAttribLocation(program_id_, "vpoint");
                glEnableVertexAttribArray(vertex_point_id);
                glVertexAttribPointer(vertex_point_id, 3, GL_FLOAT, DONT_NORMALIZE,
                                      ZERO_STRIDE, ZERO_BUFFER_OFFSET);
            }

            // texture coordinates
            {
                const GLfloat vertex_texture_coordinates[] = { /*V1*/ 0.0f, 0.0f,
                                                               /*V2*/ 1.0f, 0.0f,
                                                               /*V3*/ 0.0f, 1.0f,
                                                               /*V4*/ 1.0f, 1.0f};

                // buffer
                glGenBuffers(1, &vertex_buffer_object_);
                glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_object_);
                glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_texture_coordinates),
                             vertex_texture_coordinates, GL_STATIC_DRAW);

                // attribute
                GLuint vertex_texture_coord_id = glGetAttribLocation(program_id_,
                                                                     "vtexcoord");
                glEnableVertexAttribArray(vertex_texture_coord_id);
                glVertexAttribPointer(vertex_texture_coord_id, 2, GL_FLOAT,
                                      DONT_NORMALIZE, ZERO_STRIDE,
                                      ZERO_BUFFER_OFFSET);
            }

            {
                // load texture
                int width;
                int height;
                int nb_component;
                string filename = "waterDUDV.tga";
                // set stb_image to have the same coordinates as OpenGL
                stbi_set_flip_vertically_on_load(1);
                unsigned char* image = stbi_load(filename.c_str(), &width,
                                                 &height, &nb_component, 0);

                if(image == nullptr) {
                    throw(string("Failed to load texture"));
                }

                glGenTextures(1, &texture_dudv_id_);
                glBindTexture(GL_TEXTURE_2D, texture_dudv_id_);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

                if(nb_component == 3) {
                    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0,
                                 GL_RGB, GL_UNSIGNED_BYTE, image);
                } else if(nb_component == 4) {
                    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0,
                                 GL_RGBA, GL_UNSIGNED_BYTE, image);
                }

                texture_mirror_id_ = tex_mirror;

                // texture uniforms
                GLuint tex_dudv_id = glGetUniformLocation(program_id_, "tex_dudv");
                glUniform1i(tex_dudv_id, 0 /*GL_TEXTURE0*/);
                GLuint tex_mirror_id = glGetUniformLocation(program_id_, "tex_mirror");
                glUniform1i(tex_mirror_id, 1 /*GL_TEXTURE1*/);

                // cleanup
                glBindTexture(GL_TEXTURE_2D, 0);
                stbi_image_free(image);
            }

            move_factor_id_ = glGetUniformLocation(program_id_, "moveFactor");
            moveFactor = 0.0f;

            // to avoid the current object being polluted
            glBindVertexArray(0);
            glUseProgram(0);
        }

        void Update(float wave_speed) {
            WAVE_SPEED = wave_speed / 10000.f;
        }

        void Cleanup() {
            glBindVertexArray(0);
            glUseProgram(0);
            glDeleteBuffers(1, &vertex_buffer_object_);
            glDeleteProgram(program_id_);
            glDeleteVertexArrays(1, &vertex_array_id_);
            glDeleteTextures(1, &texture_dudv_id_);
            glDeleteTextures(1, &texture_mirror_id_);
        }

        void Draw(const glm::mat4& model, const glm::mat4& view, const glm::mat4& projection, const glm::vec3 cameraPosition) {
            glUseProgram(program_id_);
            glBindVertexArray(vertex_array_id_);

            glm::mat4 MVP = projection * view * model;

            moveFactor += WAVE_SPEED;
            moveFactor = fmod(moveFactor, 1.0f);
            glUniform1f(move_factor_id_, moveFactor);

            // Bind texture
            {
                // bind textures
                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, texture_dudv_id_);

                glActiveTexture(GL_TEXTURE1);
                glBindTexture(GL_TEXTURE_2D, texture_mirror_id_);
            }

            // setup MVP
            GLuint projectionMatrix_id = glGetUniformLocation(program_id_, "projectionMatrix");
            glUniformMatrix4fv(projectionMatrix_id, 1, GL_FALSE, value_ptr(projection));

            GLuint viewMatrix_id = glGetUniformLocation(program_id_, "viewMatrix");
            glUniformMatrix4fv(viewMatrix_id, 1, GL_FALSE, value_ptr(view));

            GLuint modelMatrix_id = glGetUniformLocation(program_id_, "modelMatrix");
            glUniformMatrix4fv(modelMatrix_id, 1, GL_FALSE, value_ptr(model));

            // setup toCameraVector
            GLuint cameraPosition_id = glGetUniformLocation(program_id_, "cameraPosition");
            glUniform3fv(cameraPosition_id, ONE, glm::value_ptr(cameraPosition));

            // draw
            glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

            glBindVertexArray(0);
            glUseProgram(0);
        }
};
