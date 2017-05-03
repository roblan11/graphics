#pragma once
#include "icg_helper.h"
#include <glm/gtc/type_ptr.hpp>

struct Terrain_Parameters {
    float mix_uw_sand;
    float mix_sand_grass;
    float lvl_grass;
    float mix_grass_rock;
    float lvl_rock;
    float mix_rock_snow;
    float lvl_snow;

    float scale_uw;
    float scale_sand;
    float scale_grass;
    float scale_rock;
    float scale_snow;

    GLuint mix_uw_sand_id;
    GLuint mix_sand_grass_id;
    GLuint lvl_grass_id;
    GLuint mix_grass_rock_id;
    GLuint lvl_rock_id;
    GLuint mix_rock_snow_id;
    GLuint lvl_snow_id;

    GLuint scale_uw_id;
    GLuint scale_sand_id;
    GLuint scale_grass_id;
    GLuint scale_rock_id;
    GLuint scale_snow_id;

    void Init(GLuint pid) {
        mix_uw_sand_id = glGetUniformLocation(pid, "mix_uw_sand");
        mix_sand_grass_id = glGetUniformLocation(pid, "mix_sand_grass");
        lvl_grass_id = glGetUniformLocation(pid, "lvl_grass");
        mix_grass_rock_id = glGetUniformLocation(pid, "mix_grass_rock");
        lvl_rock_id = glGetUniformLocation(pid, "lvl_rock");
        mix_rock_snow_id = glGetUniformLocation(pid, "mix_rock_snow");
        lvl_snow_id = glGetUniformLocation(pid, "lvl_snow");

        scale_uw_id = glGetUniformLocation(pid, "scale_uw");
        scale_sand_id = glGetUniformLocation(pid, "scale_sand");
        scale_grass_id = glGetUniformLocation(pid, "scale_grass");
        scale_rock_id = glGetUniformLocation(pid, "scale_rock");
        scale_snow_id = glGetUniformLocation(pid, "scale_snow");
    }

    void Pass(float mix_uw_sand, float mix_sand_grass, float lvl_grass, float mix_grass_rock, float lvl_rock, float mix_rock_snow, float lvl_snow, float scale_uw, float scale_sand, float scale_grass, float scale_rock, float scale_snow) {
        this->mix_uw_sand = mix_uw_sand;
        this->mix_sand_grass = mix_sand_grass;
        this->lvl_grass = lvl_grass;
        this->mix_grass_rock = mix_grass_rock;
        this->lvl_rock = lvl_rock;
        this->mix_rock_snow = mix_rock_snow;
        this->lvl_snow = lvl_snow;

        this->scale_uw = scale_uw;
        this->scale_sand = scale_sand;
        this->scale_grass = scale_grass;
        this->scale_rock = scale_rock;
        this->scale_snow = scale_snow;
    }

    void Update() {
        glUniform1f(mix_uw_sand_id, mix_uw_sand);
        glUniform1f(mix_sand_grass_id, mix_sand_grass);
        glUniform1f(lvl_grass_id, lvl_grass);
        glUniform1f(mix_grass_rock_id, mix_grass_rock);
        glUniform1f(lvl_rock_id, lvl_rock);
        glUniform1f(mix_rock_snow_id, mix_rock_snow);
        glUniform1f(lvl_snow_id, lvl_snow);

        glUniform1f(scale_uw_id, scale_uw);
        glUniform1f(scale_sand_id, scale_sand);
        glUniform1f(scale_grass_id, scale_grass);
        glUniform1f(scale_rock_id, scale_rock);
        glUniform1f(scale_snow_id, scale_snow);
    }
};

struct Light {
        // glm::vec3 La = glm::vec3(1.0f, 1.0f, 1.f);
        glm::vec3 Ld = glm::vec3(1.0f, 1.0f, 1.f);
        // glm::vec3 Ls = glm::vec3(1.0f, 1.0f, 1.f);

        glm::vec3 light_pos = glm::vec3(0.0f, 0.0f, 2.0f);

        // pass light properties to the shader
        void Setup(GLuint program_id) {
            glUseProgram(program_id);

            // given in camera space
            GLuint light_pos_id = glGetUniformLocation(program_id, "light_pos");

            // GLuint La_id = glGetUniformLocation(program_id, "La");
            GLuint Ld_id = glGetUniformLocation(program_id, "Ld");
            // GLuint Ls_id = glGetUniformLocation(program_id, "Ls");

            glUniform3fv(light_pos_id, ONE, glm::value_ptr(light_pos));
            // glUniform3fv(La_id, ONE, glm::value_ptr(La));
            glUniform3fv(Ld_id, ONE, glm::value_ptr(Ld));
            // glUniform3fv(Ls_id, ONE, glm::value_ptr(Ls));
        }
};

struct Material {
        // glm::vec3 ka = glm::vec3(0.3f, 0.3f, 0.3f);
        glm::vec3 kd = glm::vec3(0.9f, 0.9f, 0.9f);
        // glm::vec3 ks = glm::vec3(0.4f, 0.4f, 0.4f);
        // float alpha = 100.0f;

        // pass material properties to the shaders
        void Setup(GLuint program_id) {
            glUseProgram(program_id);

            // GLuint ka_id = glGetUniformLocation(program_id, "ka");
            GLuint kd_id = glGetUniformLocation(program_id, "kd");
            // GLuint ks_id = glGetUniformLocation(program_id, "ks");
            // GLuint alpha_id = glGetUniformLocation(program_id, "alpha");

            // glUniform3fv(ka_id, ONE, glm::value_ptr(ka));
            glUniform3fv(kd_id, ONE, glm::value_ptr(kd));
            // glUniform3fv(ks_id, ONE, glm::value_ptr(ks));
            // glUniform1f(alpha_id, alpha);
        }
};

class Terrain : public Material, public Light, public Terrain_Parameters {

    private:
        GLuint vertex_array_id_;                // vertex array object
        GLuint vertex_buffer_object_position_;  // memory buffer for positions
        GLuint vertex_buffer_object_index_;     // memory buffer for indices
        GLuint program_id_;                     // GLSL shader program ID
        GLuint grass_texture_id_;               // grass texture ID
        GLuint rock_texture_id_;                // rock texture ID
        GLuint sand_texture_id_;                // sand texture ID
        GLuint snow_texture_id_;                // sand texture ID
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

            {
                // set stb_image to have the same coordinates as OpenGL
                stbi_set_flip_vertically_on_load(1);

                glTexCoord2f(0.0f, 0.0f);    glVertex3f(0.0f, 0.0f, 0.0f);
                glTexCoord2f(0.0f, 1.0f);    glVertex3f(0.0f, 8.0f, 0.0f);
                glTexCoord2f(1.0f, 1.0f);    glVertex3f(8.0f, 8.0f, 0.0f);
                glTexCoord2f(1.0f, 0.0f);    glVertex3f(8.0f, 0.0f, 0.0f);

                string sand_file = "sand.tga";
                int sand_tex_width;
                int sand_tex_height;
                int sand_nb_component;
                unsigned char* sand_img = stbi_load(sand_file.c_str(), &sand_tex_width, &sand_tex_height, &sand_nb_component, 0);
                if (sand_img == nullptr) {
                  throw("Failed to load sand texture");
                }
                glGenTextures(1, &sand_texture_id_);
                glBindTexture(GL_TEXTURE_2D, sand_texture_id_);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
                if(sand_nb_component == 3) {
                    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, sand_tex_width, sand_tex_height, 0, GL_RGB, GL_UNSIGNED_BYTE, sand_img);
                } else if(sand_nb_component == 4) {
                    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, sand_tex_width, sand_tex_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, sand_img);
                }
                GLuint sand_id = glGetUniformLocation(program_id_, "sand_texture");
                glUniform1i(sand_id, 1);
                glBindTexture(GL_TEXTURE_2D, 0);
                stbi_image_free(sand_img);

                string grass_file = "grass.tga";
                int grass_tex_width;
                int grass_tex_height;
                int grass_nb_component;
                unsigned char* grass_img = stbi_load(grass_file.c_str(), &grass_tex_width, &grass_tex_height, &grass_nb_component, 0);
                if (grass_img == nullptr) {
                  throw("Failed to load grass texture");
                }
                glGenTextures(1, &grass_texture_id_);
                glBindTexture(GL_TEXTURE_2D, grass_texture_id_);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
                if(grass_nb_component == 3) {
                    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, grass_tex_width, grass_tex_height, 0, GL_RGB, GL_UNSIGNED_BYTE, grass_img);
                } else if(grass_nb_component == 4) {
                    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, grass_tex_width, grass_tex_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, grass_img);
                }
                GLuint grass_id = glGetUniformLocation(program_id_, "grass_texture");
                glUniform1i(grass_id, 2);
                glBindTexture(GL_TEXTURE_2D, 0);
                stbi_image_free(grass_img);

                string rock_file = "rock.tga";
                int rock_tex_width;
                int rock_tex_height;
                int rock_nb_component;
                unsigned char* rock_img = stbi_load(rock_file.c_str(), &rock_tex_width, &rock_tex_height, &rock_nb_component, 0);
                if (rock_img == nullptr) {
                  throw("Failed to load rock texture");
                }
                glGenTextures(1, &rock_texture_id_);
                glBindTexture(GL_TEXTURE_2D, rock_texture_id_);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
                if(rock_nb_component == 3) {
                    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, rock_tex_width, rock_tex_height, 0, GL_RGB, GL_UNSIGNED_BYTE, rock_img);
                } else if(rock_nb_component == 4) {
                    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, rock_tex_width, rock_tex_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, rock_img);
                }
                GLuint rock_id = glGetUniformLocation(program_id_, "rock_texture");
                glUniform1i(rock_id, 3);
                glBindTexture(GL_TEXTURE_2D, 0);
                stbi_image_free(rock_img);

                string snow_file = "snow.tga";
                int snow_tex_width;
                int snow_tex_height;
                int snow_nb_component;
                unsigned char* snow_img = stbi_load(snow_file.c_str(), &snow_tex_width, &snow_tex_height, &snow_nb_component, 0);
                if (snow_img == nullptr) {
                  throw("Failed to load snow texture");
                }
                glGenTextures(1, &snow_texture_id_);
                glBindTexture(GL_TEXTURE_2D, snow_texture_id_);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
                if(snow_nb_component == 3) {
                    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, snow_tex_width, snow_tex_height, 0, GL_RGB, GL_UNSIGNED_BYTE, snow_img);
                } else if(snow_nb_component == 4) {
                    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, snow_tex_width, snow_tex_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, snow_img);
                }
                GLuint snow_id = glGetUniformLocation(program_id_, "snow_texture");
                glUniform1i(snow_id, 4);
                glBindTexture(GL_TEXTURE_2D, 0);
                stbi_image_free(snow_img);
            }

            // other uniforms
            MVP_id_ = glGetUniformLocation(program_id_, "MVP");
            MV_id_ = glGetUniformLocation(program_id_, "MV");
            // to avoid the current object being polluted
            glBindVertexArray(0);
            glUseProgram(0);
        }

        void Update(float mix_uw_sand, float mix_sand_grass, float lvl_grass, float mix_grass_rock, float lvl_rock, float mix_rock_snow, float lvl_snow, float scale_uw, float scale_sand, float scale_grass, float scale_rock, float scale_snow) {
            Terrain_Parameters::Pass(mix_uw_sand, mix_sand_grass, lvl_grass, mix_grass_rock, lvl_rock, mix_rock_snow, lvl_snow, scale_uw, scale_sand, scale_grass, scale_rock, scale_snow);
        }

        void Cleanup() {
            glBindVertexArray(0);
            glUseProgram(0);
            glDeleteBuffers(1, &vertex_buffer_object_position_);
            glDeleteBuffers(1, &vertex_buffer_object_index_);
            glDeleteVertexArrays(1, &vertex_array_id_);
            glDeleteProgram(program_id_);
            glDeleteTextures(1, &sand_texture_id_);
            glDeleteTextures(1, &grass_texture_id_);
            glDeleteTextures(1, &rock_texture_id_);
            glDeleteTextures(1, &snow_texture_id_);
        }

        void Draw(const glm::mat4 &model,
                  const glm::mat4 &view,
                  const glm::mat4 &projection) {
            glUseProgram(program_id_);
            glBindVertexArray(vertex_array_id_);

            // bind textures
            // glActiveTexture(GL_TEXTURE0);
            // glBindTexture(GL_TEXTURE_2D, texture_id_);
            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, sand_texture_id_);
            glActiveTexture(GL_TEXTURE2);
            glBindTexture(GL_TEXTURE_2D, grass_texture_id_);
            glActiveTexture(GL_TEXTURE3);
            glBindTexture(GL_TEXTURE_2D, rock_texture_id_);
            glActiveTexture(GL_TEXTURE4);
            glBindTexture(GL_TEXTURE_2D, snow_texture_id_);

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
