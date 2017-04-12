#pragma once
#include "icg_helper.h"
#include <glm/gtc/type_ptr.hpp>

struct Light {
        glm::vec3 La = glm::vec3(1.0f, 1.0f, 1.f);
        glm::vec3 Ld = glm::vec3(1.0f, 1.0f, 1.f);
        glm::vec3 Ls = glm::vec3(1.0f, 1.0f, 1.f);

        glm::vec3 light_pos = glm::vec3(0.0f, 0.0f, 2.0f);

        // pass light properties to the shader
        void Setup(GLuint program_id) {
            glUseProgram(program_id);

            // given in camera space
            GLuint light_pos_id = glGetUniformLocation(program_id, "light_pos");

            GLuint La_id = glGetUniformLocation(program_id, "La");
            GLuint Ld_id = glGetUniformLocation(program_id, "Ld");
            GLuint Ls_id = glGetUniformLocation(program_id, "Ls");

            glUniform3fv(light_pos_id, ONE, glm::value_ptr(light_pos));
            glUniform3fv(La_id, ONE, glm::value_ptr(La));
            glUniform3fv(Ld_id, ONE, glm::value_ptr(Ld));
            glUniform3fv(Ls_id, ONE, glm::value_ptr(Ls));
        }
};

struct Material {
        glm::vec3 ka = glm::vec3(0.5f, 0.5f, 0.5f);
        glm::vec3 kd = glm::vec3(0.6f, 0.6f, 0.6f);
        glm::vec3 ks = glm::vec3(0.4f, 0.4f, 0.4f);
        float alpha = 100.0f;

        // pass material properties to the shaders
        void Setup(GLuint program_id) {
            glUseProgram(program_id);

            GLuint ka_id = glGetUniformLocation(program_id, "ka");
            GLuint kd_id = glGetUniformLocation(program_id, "kd");
            GLuint ks_id = glGetUniformLocation(program_id, "ks");
            GLuint alpha_id = glGetUniformLocation(program_id, "alpha");

            glUniform3fv(ka_id, ONE, glm::value_ptr(ka));
            glUniform3fv(kd_id, ONE, glm::value_ptr(kd));
            glUniform3fv(ks_id, ONE, glm::value_ptr(ks));
            glUniform1f(alpha_id, alpha);
        }
};

class Terrain : public Material, public Light {

    private:
        GLuint vertex_array_id_;                // vertex array object
        GLuint vertex_buffer_object_position_;  // memory buffer for positions
        GLuint vertex_buffer_object_index_;     // memory buffer for indices
        GLuint program_id_;                     // GLSL shader program ID
        GLuint texture_id_;                     // texture ID
        GLuint num_indices_;                    // number of vertices to render
        GLuint MVP_id_;                         // model, view, proj matrix ID
        GLuint MV_id_;                         // model, view matrix ID

    public:
        void Init(GLuint texture) {
            // compile the shaders.
            program_id_ = icg_helper::LoadShaders("terrain_vshader.glsl",
                                                  "terrain_fshader.glsl");

            if(!program_id_) {
                exit(EXIT_FAILURE);
            }

            glUseProgram(program_id_);

            // vertex one vertex array
            glGenVertexArrays(1, &vertex_array_id_);
            glBindVertexArray(vertex_array_id_);

            // vertex coordinates and indices
            {
                std::vector<GLfloat> vertices;
                std::vector<GLuint> indices;
                // TODO 5: make a triangle grid with dimension 100x100.
                // always two subsequent entries in 'vertices' form a 2D vertex position.

                int grid_dim = 512;
                float delta = 2.f / grid_dim;

                for (size_t i = 0; i <= grid_dim; i++) {
                    for (size_t j = 0; j <= grid_dim; j++) {
                        // compute vertices
                        vertices.push_back((i * delta) - 1.f);
                        vertices.push_back((j * delta) - 1.f);

                        if (i>0 && j>0) {
                            // compute indeces
                            size_t index = i*(grid_dim + 1) + j;
                            indices.push_back(index - grid_dim - 2);
                            indices.push_back(index - grid_dim - 1);
                            indices.push_back(index - 1);

                            indices.push_back(index - grid_dim - 1);
                            indices.push_back(index - 1);
                            indices.push_back(index);
                        }
                    }
                }

                num_indices_ = indices.size();

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

                // position shader attribute
                GLuint loc_position = glGetAttribLocation(program_id_, "position");
                glEnableVertexAttribArray(loc_position);
                glVertexAttribPointer(loc_position, 2, GL_FLOAT, DONT_NORMALIZE,
                                      ZERO_STRIDE, ZERO_BUFFER_OFFSET);
            }

            //assign texture
            {
            this->texture_id_ = texture;
            glBindTexture(GL_TEXTURE_2D, texture_id_);
            //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            GLuint tex_id = glGetUniformLocation(program_id_, "tex");
            glUniform1i(tex_id, 0 /*GL_TEXTURE0*/);
            glBindTexture(GL_TEXTURE_2D, 0);

            // to avoid the current object being polluted
            glBindVertexArray(0);
            glUseProgram(0);
            }

            // load texture
            {
//                int width;
//                int height;
//                int nb_component;
                //string filename = "quad_texture.tga";
                // set stb_image to have the same coordinates as OpenGL
                //stbi_set_flip_vertically_on_load(1);
                //unsigned char* image = stbi_load(filename.c_str(), &width,
                                                 //&height, &nb_component, 0);

                //if(image == nullptr) {
                    //throw(string("Failed to load texture"));
                //}

//                if(nb_component == 3) {
//                    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0,
//                                 GL_RGB, GL_UNSIGNED_BYTE, image);
//                } else if(nb_component == 4) {
//                    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0,
//                                 GL_RGBA, GL_UNSIGNED_BYTE, image);
//                }

//                GLuint tex_id = glGetUniformLocation(program_id_, "tex");
//                glUniform1i(tex_id, 0 /*GL_TEXTURE0*/);

//                // cleanup
//                glBindTexture(GL_TEXTURE_2D, 0);
//                stbi_image_free(image);
            }

            // other uniforms
            MVP_id_ = glGetUniformLocation(program_id_, "MVP");
            MV_id_ = glGetUniformLocation(program_id_, "MV");

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

        void Draw(const glm::mat4 &model,
                  const glm::mat4 &view,
                  const glm::mat4 &projection) {
            glUseProgram(program_id_);
            glBindVertexArray(vertex_array_id_);

            // bind textures
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, texture_id_);

            // setup MVP
            glm::mat4 MVP = projection*view*model;
            glUniformMatrix4fv(MVP_id_, ONE, DONT_TRANSPOSE, glm::value_ptr(MVP));

            // setup MV
            glm::mat4 MV = view*model;
            glUniformMatrix4fv(MV_id_, ONE, DONT_TRANSPOSE, glm::value_ptr(MV));

            Material::Setup(program_id_);
            Light::Setup(program_id_);

            glDrawElements(GL_TRIANGLES, num_indices_, GL_UNSIGNED_INT, 0);

            glBindVertexArray(0);
            glUseProgram(0);
        }
};
