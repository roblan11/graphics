#pragma once
#include "icg_helper.h"
#include "glm/gtc/type_ptr.hpp"

class Skyplane {

    private:
        GLuint vertex_array_id_;        // vertex array object
        GLuint program_id_;             // GLSL shader program ID
        GLuint vertex_buffer_object_;   // memory buffer
        GLuint texture_dudv_id_;        // texture ID
        GLuint texture_clouds_id_;      // texture mirror ID
        GLuint cloud_texture_id_;          // texture cloud ID
        GLuint sky_texture_id_;        // texture sky ID

    public:
        void Init(GLuint tex_clouds) {
            // compile the shaders
            program_id_ = icg_helper::LoadShaders("skyplane_vshader.glsl", "skyplane_fshader.glsl");
            if(!program_id_) {
                exit(EXIT_FAILURE);
            }

            glUseProgram(program_id_);

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

                texture_clouds_id_ = tex_clouds;

                // texture uniforms
                GLuint tex_dudv_id = glGetUniformLocation(program_id_, "tex_dudv");
                glUniform1i(tex_dudv_id, 0 /*GL_TEXTURE0*/);
                GLuint tex_clouds_id = glGetUniformLocation(program_id_, "tex_clouds");
                glUniform1i(tex_clouds_id, 1 /*GL_TEXTURE1*/);
                GLuint tex_cloud_id = glGetUniformLocation(program_id_, "tex_cloud");
                glUniform1i(tex_cloud_id, 2 /*GL_TEXTURE2*/);
                GLuint tex_sky_id = glGetUniformLocation(program_id_, "tex_sky");
                glUniform1i(tex_sky_id, 3 /*GL_TEXTURE3*/);

                // cleanup
                glBindTexture(GL_TEXTURE_2D, 0);
                stbi_image_free(image);

                string cloud_file = "cloud.tga";
                int cloud_tex_width;
                int cloud_tex_height;
                int cloud_nb_component;
                unsigned char* cloud_img = stbi_load(cloud_file.c_str(), &cloud_tex_width, &cloud_tex_height, &cloud_nb_component, 0);
                if (cloud_img == nullptr) {
                    throw("Failed to load cloud texture");
                }
                glGenTextures(1, &cloud_texture_id_);
                glBindTexture(GL_TEXTURE_2D, cloud_texture_id_);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
                if (cloud_nb_component == 3) {
                    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, cloud_tex_width, cloud_tex_height, 0, GL_RGB, GL_UNSIGNED_BYTE, cloud_img);
                } else if (cloud_nb_component == 4) {
                    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, cloud_tex_width, cloud_tex_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, cloud_img);
                }
                GLuint cloud_id = glGetUniformLocation(program_id_, "cloud_texture");
                glUniform1i(cloud_id, 2);
                glBindTexture(GL_TEXTURE_2D, 0);
                stbi_image_free(cloud_img);

                string sky_file = "sky.tga";
                int sky_tex_width;
                int sky_tex_height;
                int sky_nb_component;
                unsigned char* sky_img = stbi_load(sky_file.c_str(), &sky_tex_width, &sky_tex_height, &sky_nb_component, 0);
                if (sky_img == nullptr) {
                    throw("Failed to load sky texture");
                }
                glGenTextures(1, &sky_texture_id_);
                glBindTexture(GL_TEXTURE_2D, sky_texture_id_);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
                if (sky_nb_component == 3) {
                    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, sky_tex_width, sky_tex_height, 0, GL_RGB, GL_UNSIGNED_BYTE, sky_img);
                } else if (sky_nb_component == 4) {
                    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, sky_tex_width, sky_tex_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, sky_img);
                }
                GLuint sky_id = glGetUniformLocation(program_id_, "sky_texture");
                glUniform1i(sky_id, 2);
                glBindTexture(GL_TEXTURE_2D, 0);
                stbi_image_free(sky_img);
            }

            // to avoid the current object being polluted
            glBindVertexArray(0);
            glUseProgram(0);
        }

        void Cleanup() {
            glBindVertexArray(0);
            glUseProgram(0);
            glDeleteBuffers(1, &vertex_buffer_object_);
            glDeleteProgram(program_id_);
            glDeleteVertexArrays(1, &vertex_array_id_);
            glDeleteTextures(1, &texture_dudv_id_);
            glDeleteTextures(1, &texture_clouds_id_);
            glDeleteTextures(1, &sky_texture_id_);
            glDeleteTextures(1, &cloud_texture_id_);
        }

        void Draw(const glm::mat4& model, const glm::mat4& view, const glm::mat4& projection) {
            glUseProgram(program_id_);
            glBindVertexArray(vertex_array_id_);

            glm::mat4 MVP = projection * view * model;

            // Bind texture
            {
                // bind textures
                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, texture_dudv_id_);

                glActiveTexture(GL_TEXTURE1);
                glBindTexture(GL_TEXTURE_2D, texture_clouds_id_);

                glActiveTexture(GL_TEXTURE2);
                glBindTexture(GL_TEXTURE_2D, cloud_texture_id_);

                glActiveTexture(GL_TEXTURE3);
                glBindTexture(GL_TEXTURE_2D, sky_texture_id_);
            }

            // setup MVP
            GLuint MVP_id = glGetUniformLocation(program_id_, "MVP");
            glUniformMatrix4fv(MVP_id, 1, GL_FALSE, value_ptr(MVP));

            // draw
            glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

            glBindVertexArray(0);
            glUseProgram(0);
        }
};
