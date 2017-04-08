#pragma once
#include "icg_helper.h"

class HeightMap {

    private:
        GLuint vertex_array_id_;        // vertex array object
        GLuint program_id_;             // GLSL shader program ID
        GLuint vertex_buffer_object_;   // memory buffer
        GLuint texture_id_;             // texture ID

        float screenquad_width_;
        float screenquad_height_;

        float* kernel;
        size_t kLength;
        float stdeviation;

    public:
        void setStandardDeviation(float stdeviationInput) {
            this->stdeviation = stdeviationInput;
            this->kLength = 1 + 2 * 3 * int(ceil(this->stdeviation));
            int mid = this->kLength >> 1;
            for (int i = 0; i <= mid; ++i) {
                double tmp = exp(- (i * i) / ( 2.0 * this->stdeviation * this->stdeviation));
                kernel[mid + i] = tmp;
                kernel[mid - i] = tmp;
            }
        }

        void Init(float screenquad_width, float screenquad_height,
                  GLuint texture) {

            // set screenquad size
            this->screenquad_width_ = screenquad_width;
            this->screenquad_height_ = screenquad_height;

            // compile the shaders
            program_id_ = icg_helper::LoadShaders("heightmap_vshader.glsl",
                                                  "heightmap_fshader.glsl");
            if(!program_id_) {
                exit(EXIT_FAILURE);
            }

            glUseProgram(program_id_);

            // vertex one vertex Array
            glGenVertexArrays(1, &vertex_array_id_);
            glBindVertexArray(vertex_array_id_);

            //////// Start
            this->kernel = new float[1024]; // otherwise: error: unsized array index must be constant
            setStandardDeviation(2.0);
            //////// End

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

            // load/Assign texture
            this->texture_id_ = texture;
            glBindTexture(GL_TEXTURE_2D, texture_id_);
            GLuint tex_id = glGetUniformLocation(program_id_, "tex");
            glUniform1i(tex_id, 0 /*GL_TEXTURE0*/);
            glBindTexture(GL_TEXTURE_2D, 0);

            int p[256] = { 171, 131, 182, 5, 239, 54, 114, 208, 130, 149, 209, 25, 161, 147, 251, 175, 22, 147, 127, 237, 200, 176, 166, 113, 11, 45, 132, 134, 216, 75, 99, 148, 67, 96, 46, 168, 15, 58, 232, 130, 87, 39, 128, 235, 75, 44, 234, 148, 71, 190, 25, 109, 242, 122, 70, 53, 185, 122, 170, 150, 201, 173, 249, 65, 65, 124, 88, 14, 34, 222, 29, 84, 216, 208, 201, 22, 67, 70, 72, 24, 190, 160, 151, 101, 16, 169, 60, 14, 219, 69, 48, 185, 243, 202, 89, 234, 72, 228, 44, 244, 114, 174, 102, 85, 152, 65, 247, 5, 254, 184, 206, 217, 122, 81, 91, 148, 111, 151, 177, 161, 24, 95, 217, 147, 219, 157, 205, 141, 49, 7, 243, 83, 89, 45, 22, 136, 208, 63, 171, 107, 217, 103, 153, 58, 10, 200, 76, 5, 36, 76, 134, 222, 213, 188, 132, 179, 53, 194, 216, 248, 84, 167, 212, 98, 227, 4, 1, 142, 64, 45, 239, 212, 24, 196, 53, 99, 233, 138, 157, 32, 69, 125, 232, 143, 163, 165, 142, 128, 59, 176, 147, 65, 55, 11, 79, 101, 1, 243, 183, 28, 133, 1, 213, 142, 188, 26, 106, 126, 99, 242, 171, 3, 88, 14, 203, 4, 200, 209, 133, 60, 98, 100, 191, 56, 10, 216, 11, 189, 135, 104, 86, 174, 214, 163, 70, 20, 133, 148, 185, 219, 55, 12, 98, 40, 176, 40, 161, 122, 129, 52, 86, 85, 33, 17, 173, 62 };
            glUseProgram(program_id_);
            GLuint p_id = glGetUniformLocation(program_id_, "p");
            glUniform1iv(p_id, 256, p);

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
            glDeleteTextures(1, &texture_id_);
            delete [] kernel;
        }

        void UpdateSize(int screenquad_width, int screenquad_height) {
            this->screenquad_width_ = screenquad_width;
            this->screenquad_height_ = screenquad_height;
        }

        void IncreaseSigma() {
            double variance = 0.25;
            double sigma = sqrt(0.25);
            double newStdDev = this->stdeviation + sigma;
            if (newStdDev < 4.1) {
                setStandardDeviation(newStdDev);
            }
        }

        void DecreaseSigma() {
            double variance = 0.25;
            double sigma = sqrt(0.25);
            double newStdDev = this->stdeviation - sigma;
            if (newStdDev > 0.0) {
                setStandardDeviation(newStdDev);
            }
        }

        void Draw() {
            glUseProgram(program_id_);
            glBindVertexArray(vertex_array_id_);

            // window size uniforms
            glUniform1f(glGetUniformLocation(program_id_, "tex_width"),
                        this->screenquad_width_);
            glUniform1f(glGetUniformLocation(program_id_, "tex_height"),
                        this->screenquad_height_);

            // bind texture
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, texture_id_);

            // draw
            glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

            glBindVertexArray(0);
            glUseProgram(0);
        }
};
