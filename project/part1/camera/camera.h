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

class Camera : public Bezier {

    public:
        void Init(vec3 position, vec3 lookingAt, vec3 up);

        mat4 ViewMatrix(bool reflection);

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

        void InitBezier(float time);

        void MoveBezier(float time);

        vec3 ComputeUpVector(vec3 pos, vec3 lookAt);

        void CorrectUpVector();

        void Cleanup();

        //void Draw();

    private:
        vec3 position_;
        vec3 lookingAt_;
        vec3 up_;
        float velocity_;
        bool bezierInitialized;
};

#endif
