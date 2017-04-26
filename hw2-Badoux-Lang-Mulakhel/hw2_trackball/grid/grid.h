#pragma once
#include "icg_helper.h"
#include <glm/gtc/type_ptr.hpp>

// enable / disable the water effect here ======================================
// to do this, set the boolean value to true / false
#define ENABLE_WATER_EFFECT true

class Grid {

    private:
        GLuint vertex_array_id_;                // vertex array object
        GLuint vertex_buffer_object_position_;  // memory buffer for positions
        GLuint vertex_buffer_object_index_;     // memory buffer for indices
        GLuint program_id_;                     // GLSL shader program ID
        GLuint texture_id_;                     // texture ID
        GLuint num_indices_;                    // number of vertices to render
        GLuint MVP_id_;                         // model, view, proj matrix ID

    public:
        void CreateGrid(size_t grid_dim, float grid_start, float grid_end) {
            std::vector<GLfloat> vertices;
            std::vector<GLuint> indices;

            float delta = (grid_end - grid_start) / grid_dim;

            for (size_t i = 0; i <= grid_dim; i++) {
                for (size_t j = 0; j <= grid_dim; j++) {
                    // compute vertices
                    vertices.push_back((i * delta) + grid_start);
                    vertices.push_back((j * delta) + grid_start);
                }
            }

            for (int i = 0; i < grid_dim; i++) {
                if ((i&1) == 0) {
                    for (int j = 0; j <= grid_dim; j++) {
                        indices.push_back(j + (grid_dim+1)*i);
                        indices.push_back(j + (grid_dim+1)*(i+1));
                    }
                } else {
                    for (int j = grid_dim; j >= 0; j--) {
                        indices.push_back(j + (grid_dim+1)*i);
                        indices.push_back(j + (grid_dim+1)*(i+1));
                    }
                }
            }

            // old algorithm, adds about 3x more indeces
            // for (size_t i = 0; i <= grid_dim; i++) {
            //     for (size_t j = 0; j <= grid_dim; j++) {
            //         // compute vertices
            //         vertices.push_back((i * delta) + grid_start);
            //         vertices.push_back((j * delta) + grid_start);
            //
            //         if (i>0 && j>0) {
            //             // compute indeces
            //             size_t index = i*(grid_dim + 1) + j;
            //             indices.push_back(index - grid_dim - 2);
            //             indices.push_back(index - grid_dim - 1);
            //             indices.push_back(index - 1);
            //
            //             indices.push_back(index - grid_dim - 1);
            //             indices.push_back(index - 1);
            //             indices.push_back(index);
            //         }
            //     }
            // }

            num_indices_ = indices.size();
            std::cout << num_indices_ << '\n';

            // position buffer
            glGenBuffers(1, &vertex_buffer_object_position_);
            glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_object_position_);
            glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat),
                         &vertices[0], GL_STATIC_DRAW);

            // vertex indices
            glGenBuffers(1, &vertex_buffer_object_index_);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vertex_buffer_object_index_);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint),
                         &indices[0], GL_STATIC_DRAW);
        }

        void Init() {
            // compile the shaders.
            program_id_ = icg_helper::LoadShaders("grid_vshader.glsl",
                                                  "grid_fshader.glsl");

            if(!program_id_) {
                exit(EXIT_FAILURE);
            }

            glUseProgram(program_id_);

            glUniform1i(glGetUniformLocation(program_id_, "water"), ENABLE_WATER_EFFECT);

            // vertex one vertex array
            glGenVertexArrays(1, &vertex_array_id_);
            glBindVertexArray(vertex_array_id_);

            // vertex coordinates and indices
            {
                CreateGrid(512, -1.f, 1.f);

                // position shader attribute
                GLuint loc_position = glGetAttribLocation(program_id_, "position");
                glEnableVertexAttribArray(loc_position);
                glVertexAttribPointer(loc_position, 2, GL_FLOAT, DONT_NORMALIZE,
                                      ZERO_STRIDE, ZERO_BUFFER_OFFSET);
            }

            // load texture
            {
                int width;
                int height;
                int nb_component;
                string filename = "grid_texture.tga";
                // set stb_image to have the same coordinates as OpenGL
                stbi_set_flip_vertically_on_load(1);
                unsigned char* image = stbi_load(filename.c_str(), &width,
                                                 &height, &nb_component, 0);

                if(image == nullptr) {
                    throw(string("Failed to load texture"));
                }

                glGenTextures(1, &texture_id_);
                glBindTexture(GL_TEXTURE_2D, texture_id_);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

                if(nb_component == 3) {
                    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0,
                                 GL_RGB, GL_UNSIGNED_BYTE, image);
                } else if(nb_component == 4) {
                    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0,
                                 GL_RGBA, GL_UNSIGNED_BYTE, image);
                }

                GLuint tex_id = glGetUniformLocation(program_id_, "tex");
                glUniform1i(tex_id, 0 /*GL_TEXTURE0*/);

                // cleanup
                glBindTexture(GL_TEXTURE_2D, 0);
                stbi_image_free(image);
            }

            // other uniforms
            MVP_id_ = glGetUniformLocation(program_id_, "MVP");

            // to avoid the current object being polluted
            glBindVertexArray(0);
            glUseProgram(0);
        }

        void Cleanup() {
            glBindVertexArray(0);
            glUseProgram(0);
            glDeleteBuffers(1, &vertex_buffer_object_position_);
            glDeleteBuffers(1, &vertex_buffer_object_index_);
            glDeleteVertexArrays(1, &vertex_array_id_);
            glDeleteProgram(program_id_);
            glDeleteTextures(1, &texture_id_);
        }

        void Draw(float time, const glm::mat4 &model = IDENTITY_MATRIX,
                  const glm::mat4 &view = IDENTITY_MATRIX,
                  const glm::mat4 &projection = IDENTITY_MATRIX) {
            glUseProgram(program_id_);
            glBindVertexArray(vertex_array_id_);

            // bind textures
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, texture_id_);

            // setup MVP
            glm::mat4 MVP = projection*view*model;
            glUniformMatrix4fv(MVP_id_, ONE, DONT_TRANSPOSE, glm::value_ptr(MVP));

            // pass the current time stamp to the shader.
            glUniform1f(glGetUniformLocation(program_id_, "time"), time);

            // draw
            // TODO 5: for debugging it can be helpful to draw only the wireframe.
            // You can do that by uncommenting the next line.
            //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            // TODO 5: depending on how you set up your vertex index buffer, you
            // might have to change GL_TRIANGLE_STRIP to GL_TRIANGLES.
            glDrawElements(GL_TRIANGLE_STRIP, num_indices_, GL_UNSIGNED_INT, 0);

            glBindVertexArray(0);
            glUseProgram(0);
        }
};
