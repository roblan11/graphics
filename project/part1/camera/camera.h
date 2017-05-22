#ifndef DEF_CAMERA
#define DEF_CAMERA


#pragma once
//#include "icg_helper.h"
#include <glm/gtc/type_ptr.hpp>

class Camera {

    public:
        void Init(glm::vec3 position, glm::vec3 lookingAt, glm::vec3 up);

        glm::mat4 ViewMatrix(bool reflection);

        float getPositionX() {
            return position_.x;
        }

        float getPositionY() {
            return position_.y;
        }

        void MoveForward(float currentTime);
        void MovingForward(float currentTime);

        void MoveBackward(float currentTime);
        void MovingBackward(float currentTime);

        void LookOnTheLeft(float currentTime);
        void LookingOnTheLeft(float currentTime);

        void LookOnTheRight(float currentTime);
        void LookingOnTheRight(float currentTime);

        void LookAbove(float currentTime);
        void LookingAbove(float currentTime);

        void LookBelow(float currentTime);
        void LookingBelow(float currentTime);

        glm::vec3 ComputeUpVector(glm::vec3 pos, glm::vec3 lookAt);

        void CorrectUpVector();

        void Cleanup();

        void Update(float currentTime);

    private:
        void UpdateOrigin(float currentTime);

        glm::vec3 position_;
        glm::vec3 lookingAt_;
        glm::vec3 up_;
        glm::vec3 positionOrigin_;
        glm::vec3 lookingAtOrigin_;
        glm::vec3 velocityPositionOrigin_;
        glm::vec3 velocityLookingAtOrigin_;
        float timeOriginPosition_;
        float timeOriginLookingAt_;
        static const float MOVE_PITCH_FACTOR;
        static const float MOVE_YAW_FACTOR;
        static const float MOVE_STRAIGHT_FACTOR;
        static const float INITIAL_VELOCITY;
        static const float ACCELERATION;
};

#endif
