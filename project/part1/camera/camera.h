#ifndef DEF_CAMERA
#define DEF_CAMERA


#pragma once
//#include "icg_helper.h"
#include <glm/gtc/type_ptr.hpp>
using namespace glm;

struct Bezier {
    vec3* points;
    int numPoints;
    float runFor;
    float runFrom;
    bool restart;

    void Init(vec3* pts, size_t N, float runTime, float startTime);

    float Ease(float t);

    vec3 GetPosition(float time);

    void Cleanup();
};

enum class CameraMode {BIRD, FPS, BEZIER};

class Camera : public Bezier {

    public:
        void Init(vec3 position, vec3 lookingAt, vec3 up);

        mat4 ViewMatrix(bool reflection);

        void SetMode(CameraMode newMode) {
            mode = newMode;
        }

        float getPositionX() {
            return position_.x;
        }

        float getPositionY() {
            return position_.y;
        }

        glm::vec2 getWorldCenterPosition();

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

        void InitBezier(float time);

        void MoveBezier(float time);

        void CorrectUpVector();

        void Cleanup();

        void Update(float currentTime);

    private:
        void UpdateOrigin(float currentTime);

        vec3 ComputeUpVector(vec3 pos, vec3 lookAt);

        glm::vec3 position_;
        glm::vec3 lookingAt_;
        glm::vec3 up_;
        glm::vec3 positionOrigin_;
        glm::vec3 lookingAtOrigin_;
        glm::vec3 velocityPositionOrigin_;
        glm::vec3 velocityLookingAtOrigin_;
        bool bezierInitialized;
        float velocity_;
        float timeOriginPosition_;
        float timeOriginLookingAt_;
        CameraMode mode;
        static const float MOVE_PITCH_FACTOR;
        static const float MOVE_YAW_FACTOR;
        static const float MOVE_STRAIGHT_FACTOR;
        static const float INITIAL_VELOCITY;
        static const float ACCELERATION;
};

#endif
