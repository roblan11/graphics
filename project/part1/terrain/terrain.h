#pragma once
#include "icg_helper.h"
#include <glm/gtc/type_ptr.hpp>

struct Terrain_Parameters {
    float lvl_sand;
    float mix_sand_grass;
    float lvl_grass;
    float mix_grass_snow;
    float lvl_snow;

    GLuint lvl_sand_id;
    GLuint mix_sand_grass_id;
    GLuint lvl_grass_id;
    GLuint mix_grass_snow_id;
    GLuint lvl_snow_id;

    void Init(GLuint pid) {
        lvl_sand_id = glGetUniformLocation(pid, "lvl_sand");
        mix_sand_grass_id = glGetUniformLocation(pid, "mix_sand_grass");
        lvl_grass_id = glGetUniformLocation(pid, "lvl_grass");
        mix_grass_snow_id = glGetUniformLocation(pid, "mix_grass_snow");
        lvl_snow_id = glGetUniformLocation(pid, "lvl_snow");
    }

    void Pass(float lvl_sand, float mix_sand_grass, float lvl_grass, float mix_grass_snow, float lvl_snow) {
        this->lvl_sand = lvl_sand;
        this->mix_sand_grass = mix_sand_grass;
        this->lvl_grass = lvl_grass;
        this->mix_grass_snow = mix_grass_snow;
        this->lvl_snow = lvl_snow;
    }

    void Update() {
        glUniform1f(lvl_sand_id, lvl_sand);
        glUniform1f(mix_sand_grass_id, mix_sand_grass);
        glUniform1f(lvl_grass_id, lvl_grass);
        glUniform1f(mix_grass_snow_id, mix_grass_snow);
        glUniform1f(lvl_snow_id, lvl_snow);
    }
};

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

class Terrain : public Material, public Light, public Terrain_Parameters {

    private:
        GLuint vertex_array_id_;                // vertex array object
        GLuint vertex_buffer_object_position_;  // memory buffer for positions
        GLuint vertex_buffer_object_index_;     // memory buffer for indices
        GLuint program_id_;                     // GLSL shader program ID
        GLuint texture_id_;                     // texture ID
        GLuint num_indices_;                    // number of vertices to render
        GLuint MVP_id_;                         // model, view, proj matrix ID
        GLuint MV_id_;                         // model, view matrix ID

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

            num_indices_ = indices.size();
            std::cout << num_indices_ << " indices" << '\n';

            // position buffer
            glGenBuffers(1, &vertex_buffer_object_position_);
            glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_object_position_);
            glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), &vertices[0], GL_STATIC_DRAW);

            // vertex indices
            glGenBuffers(1, &vertex_buffer_object_index_);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vertex_buffer_object_index_);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, num_indices_ * sizeof(GLuint), &indices[0], GL_STATIC_DRAW);
        }

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

            Terrain_Parameters::Init(program_id_);

            // vertex coordinates and indices
            {
                CreateGrid(2048, -1.f, 1.f);

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

            }

            // other uniforms
            MVP_id_ = glGetUniformLocation(program_id_, "MVP");
            MV_id_ = glGetUniformLocation(program_id_, "MV");
            // to avoid the current object being polluted
            glBindVertexArray(0);
            glUseProgram(0);
        }

        void Update(float lvl_sand, float mix_sand_grass, float lvl_grass, float mix_grass_snow, float lvl_snow) {
            Terrain_Parameters::Pass(lvl_sand, mix_sand_grass, lvl_grass, mix_grass_snow, lvl_snow);
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

            Terrain_Parameters::Update();

            Material::Setup(program_id_);
            Light::Setup(program_id_);

            glDrawElements(GL_TRIANGLE_STRIP, num_indices_, GL_UNSIGNED_INT, 0);

            glBindVertexArray(0);
            glUseProgram(0);
        }
};

