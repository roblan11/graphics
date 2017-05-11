#include "camera.h"
#include "glm/gtc/matrix_transform.hpp"

#include <iostream>

using namespace glm;

using namespace std;

#define MOVE_VERTICALLY_FACTOR 0.05
#define MOVE_LATERAL_FACTOR 0.1f
#define MOVE_STRAIGHT_FACTOR 0.2f

void Camera::Init(vec3 position, vec3 lookingAt, vec3 up)
{
    position_ = position;
    lookingAt_ = lookingAt;
    up_ = up;
    velocity_ = 0.0f;
    // cout << "Hello world!" << endl;
}

mat4 Camera::ViewMatrix(bool reflection)
{
    if (reflection) {
        float dz = position_.z - lookingAt_.z;
        float k = position_.z / dz; // asssuming water at z=0
        vec3 lookingAtBis = position_ + k * (lookingAt_ - position_);
        vec3 cam_pos_mirrored = vec3(position_.x, position_.y, -position_.z);
        vec3 up = ComputeUpVector(cam_pos_mirrored, lookingAtBis);
        return lookAt(cam_pos_mirrored, lookingAtBis, up);
    } else {
        return lookAt(position_, lookingAt_, up_);
    }
}

void Camera::MoveForward()
{
    vec3 dmove = normalize(lookingAt_ - position_);
    dmove.z = 0.0f;
    position_ += MOVE_STRAIGHT_FACTOR * dmove;
    lookingAt_ += MOVE_STRAIGHT_FACTOR * dmove;
}

void Camera::MoveBackward()
{
    vec3 dmove = normalize(lookingAt_ - position_);
    dmove.z = 0.0f;
    position_ -= MOVE_STRAIGHT_FACTOR * dmove;
    lookingAt_ -= MOVE_STRAIGHT_FACTOR * dmove;
}

void Camera::LookOnTheLeft()
{
    vec3 cameraPositionToLookingAt = lookingAt_ - position_;
    vec3 horizontalAxis = normalize(cross(vec3(0.0f, 0.0f, 1.0f), cameraPositionToLookingAt));
    lookingAt_ += mat3(0.1) * horizontalAxis;
}

void Camera::LookOnTheRight()
{
    vec3 cameraPositionToLookingAt = lookingAt_ - position_;
    vec3 horizontalAxis = normalize(cross(vec3(0.0f, 0.0f, 1.0f), cameraPositionToLookingAt));
    lookingAt_ -= mat3(0.1) * horizontalAxis;
}

vec3 Camera::ComputeUpVector(vec3 pos, vec3 lookAt)
{
    vec3 axeZ = vec3(0.0f, 0.0f, 1.0f);
    vec3 positionToLookingAt = lookAt - pos;
    vec3 crossHorizontal = cross(axeZ, positionToLookingAt);
    return cross(positionToLookingAt, crossHorizontal);
}

void Camera::CorrectUpVector()
{
    up_ = ComputeUpVector(position_, lookingAt_);
}
