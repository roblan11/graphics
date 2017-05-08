#ifndef DEF_CAMERA
#define DEF_CAMERA


#pragma once
//#include "icg_helper.h"
#include <glm/gtc/type_ptr.hpp>

class Camera {

    public:
        void Init(glm::vec3 position, glm::vec3 lookingAt, glm::vec3 up);

        glm::mat4 ViewMatrix(bool reflection);

        float getPositionLookingAtX() {
            return lookingAt_.x;
        }

        float getPositionLookingAtY() {
            return lookingAt_.y;
        }

        void MoveForward();

        void MoveBackward();

        void LookOnTheLeft();

        void LookOnTheRight();

        glm::vec3 ComputeUpVector(glm::vec3 pos, glm::vec3 lookAt);

        void CorrectUpVector();

        void Cleanup();

        void Draw();

    private:
        glm::vec3 position_;
        glm::vec3 lookingAt_;
        glm::vec3 up_;
        float velocity_;
};

#endif
