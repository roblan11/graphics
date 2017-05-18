#include "camera.h"
#include "glm/gtc/matrix_transform.hpp"

#include <iostream>

using namespace glm;

#define MOVE_VERTICALLY_FACTOR 0.05
#define MOVE_LATERAL_FACTOR 0.1f
#define MOVE_STRAIGHT_FACTOR 0.2f

void Bezier::Init(vec3 pts[4], float runTime, float startTime) {
    points = new vec3[numPoints];
    memcpy(points, pts, 4 * sizeof(vec3));
    restart = false;
    numPoints = 4;
    runFor = runTime;
    runFrom = startTime;
}

float Bezier::Ease(float t) {
  return t * t * t * (t * (t * 6 - 15) + 10);
}

vec3 Bezier::GetPosition(float time) {
    float t;
    if(time >= runFrom+runFor) {
        restart = true;
        t = 1.0;
    } else {
        float t = Ease((time-runFrom)/runFor);
    }
    vec3* temp = new vec3[numPoints];
    memcpy(temp, points, numPoints * sizeof(vec3));
    for (int i = numPoints-1; i > 0; i--) {
        for (size_t j = 0; j < i; j++) {
            temp[j] = temp[j] + t * (temp[j+1] - temp[j]);
        }
    }
    vec3 ret = temp[0];
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

void Camera::InitBezier(float time) {
    vec3 points[4] = {vec3(0.0, 0.0, 2.0), vec3(1.0, 1.0, 1.0), vec3(0.0, 1.0, 2.0), vec3(0.0, 0.0, 2.0)};
    Bezier::Init(points, 10, time);
    bezierInitialized = true;
}

void Camera::MoveBezier(float time) {
    position_ = vec3(Bezier::GetPosition(time));
    if(Bezier::restart) {
        InitBezier(time);
    }
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
