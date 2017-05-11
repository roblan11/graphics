#include "camera.h"
#include "glm/gtc/matrix_transform.hpp"

#include <iostream>

using namespace glm;

#define MOVE_VERTICALLY_FACTOR 0.05
#define MOVE_LATERAL_FACTOR 0.1f
#define MOVE_STRAIGHT_FACTOR 0.2f

void Bezier::Init(vec2* pts, int N, float maxTime, float startTime) {
    points = new vec2[numPoints];
    memcpy(points, pts, N * sizeof(vec2));
    numPoints = N;
    runFor = maxTime;
}

vec2 Bezier::GetPosition(float time) {
    float t = clamp((time-runFrom)/runFor, 0.f, 1.f);
    vec2* temp = new vec2[numPoints];
    memcpy(temp, points, numPoints * sizeof(vec2));
    for (int i = numPoints-1; i > 0; i--) {
        for (size_t j = 0; j < i; j++) {
            temp[j] = temp[j] + t * (temp[j+1] - temp[j]);
        }
    }
    vec2 ret = temp[0];
    delete[] temp;
    return ret;
}

void Bezier::Cleanup() {
    delete[] points;
}

void Camera::Init(vec3 position, vec3 lookingAt, vec3 up)
{
    position_ = position;
    lookingAt_ = lookingAt;
    up_ = up;
    velocity_ = 0.0f;

    // until properly implemented
    vec2 points[] = {vec2(1.0, 1.0), vec2(5.0, 5.0), vec2(1.0, 5.0)};
    Bezier::Init(points, 3, 10.0, 2.0);
}

mat4 Camera::ViewMatrix(bool reflection)
{
    if (reflection) {
        vec3 cam_pos_mirrored = vec3(position_.x, position_.y, -position_.z);
        vec3 up = ComputeUpVector(cam_pos_mirrored, lookingAt_);
        return lookAt(cam_pos_mirrored, lookingAt_, up);
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

void Camera::Cleanup() {
  if (bezierInitialized) {
      Bezier::Cleanup();
  }
}
