#include "camera.h"
#include "glm/gtc/matrix_transform.hpp"

#include <iostream>

using namespace glm;

using namespace std;

const float Camera::MOVE_PITCH_FACTOR = 3.1f; // around x if aimingVec = (0,1,0)
const float Camera::MOVE_YAW_FACTOR = 3.1f; // arount z
const float Camera::MOVE_STRAIGHT_FACTOR = 0.05f;
const float Camera::INITIAL_VELOCITY= 0.3f;
const float Camera::ACCELERATION = 0.2f;

void Bezier::Init(vec3* posPts, vec3* lookPts, size_t N, float runTime, float startTime) {
    BnumPoints = N;
    BposPoints = new vec3[BnumPoints];
    memcpy(BposPoints, posPts, BnumPoints * sizeof(vec3));
    BlookPoints = new vec3[BnumPoints];
    memcpy(BlookPoints, lookPts, BnumPoints * sizeof(vec3));
    Brestart = false;
    BrunFor = runTime;
    BrunFrom = startTime;
    if (N > 0) {
        Bposition = BposPoints[0];
        BlookAt = BlookPoints[0];
    }
}

float Bezier::Ease(float t) {
  return t * t * t * (t * (t * 6 - 15) + 10);
}

void Bezier::UpdatePosition(float time) {
    float t;
    if(time >= BrunFrom+BrunFor) {
        Brestart = true;
        t = 1.0;
    } else if (time >= BrunFrom) {
        t = Ease((time-BrunFrom)/BrunFor);
    }

    vec3* posTmp = new vec3[BnumPoints];
    memcpy(posTmp, BposPoints, BnumPoints * sizeof(vec3));
    vec3* lookTmp = new vec3[BnumPoints];
    memcpy(lookTmp, BlookPoints, BnumPoints * sizeof(vec3));

    for (int i = BnumPoints-1; i > 0; i--) {
        for (size_t j = 0; j < i; j++) {
            posTmp[j] = posTmp[j] + t * (posTmp[j+1] - posTmp[j]);
            lookTmp[j] = lookTmp[j] + t * (lookTmp[j+1] - lookTmp[j]);
        }
    }
    Bposition = posTmp[0];
    delete[] posTmp;
    BlookAt = lookTmp[0];
    delete[] lookTmp;
}

void Bezier::Cleanup() {
  delete[] BposPoints;
  delete[] BlookPoints;
}

void Camera::Init(vec3 position, vec3 lookingAt, vec3 up)
{
    position_ = position;
    positionOrigin_ = position;
    lookingAt_ = lookingAt;
    lookingAtOrigin_ = lookingAt;
    up_ = up;
    mode = CameraMode::BIRD;
    velocity_ = 0.0f;
    velocityPositionOrigin_ = vec3(0.0f);
    velocityLookingAtOrigin_ = vec3(0.0f);
    timeOriginPosition_ = 0.0f;
    timeOriginLookingAt_ = 0.0f;
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

void Camera::Update(float currentTime)
{
    //if (mode == CameraMode::FPS) {
        // FPS
    //} else
    if (mode == CameraMode::BIRD || mode == CameraMode::FPS) {
        float position_dt = currentTime - timeOriginPosition_;
        float position_dt_dt = position_dt * position_dt;
        position_ = positionOrigin_ + mat3(INITIAL_VELOCITY * position_dt -
                0.5 * ACCELERATION * position_dt_dt) * velocityPositionOrigin_;
        if (INITIAL_VELOCITY < ACCELERATION * position_dt) {
            positionOrigin_ = position_;
            velocityPositionOrigin_ = vec3(0.0f);
            timeOriginPosition_ = currentTime;
        }
        float lookingAt_dt = currentTime - timeOriginLookingAt_;
        float lookingAt_dt_dt = lookingAt_dt *  lookingAt_dt;
        lookingAt_ = lookingAtOrigin_ + mat3(INITIAL_VELOCITY * lookingAt_dt -
                0.5 * ACCELERATION * lookingAt_dt_dt) * velocityLookingAtOrigin_;
        if (INITIAL_VELOCITY < ACCELERATION * lookingAt_dt) {
            lookingAtOrigin_ = lookingAt_;
            velocityLookingAtOrigin_ = vec3(0.0f);
            timeOriginLookingAt_ = currentTime;
        }
        CorrectSphericalCoordinates();
        CorrectLookingAt();
    } else {
        MoveBezier(currentTime);
    }
}

vec2 Camera::getWorldCenterPosition()
{
    const float offset = 5.5f;
    vec3 op = lookingAt_ - position_;
    op.z = position_.z;
    vec3 tmp = position_ + mat3(offset) * normalize(op);
    vec2 ret = vec2(tmp.x, tmp.y);
    return ret;
}

void Camera::MoveForward(float currentTime)
{
    vec3 dmove = normalize(lookingAt_ - position_);
    velocityPositionOrigin_ = dmove;
    velocityLookingAtOrigin_ = dmove;
    UpdateOrigin(currentTime);
}

void Camera::MovingForward(float currentTime)
{
    if (currentTime - timeOriginPosition_ > 0.1) {
        UpdateOrigin(currentTime);
    }
}

void Camera::MoveBackward(float currentTime)
{
    vec3 dmove = normalize(position_ - lookingAt_);
    velocityPositionOrigin_ = dmove;
    velocityLookingAtOrigin_ = dmove;
    UpdateOrigin(currentTime);
}

void Camera::MovingBackward(float currentTime)
{
    if (currentTime - timeOriginPosition_ > 0.1) {
        UpdateOrigin(currentTime);
    }
}

void Camera::MoveUpDown(float currentTime, float direction)
{
    vec3 dmove = vec3(0, 0, 2*direction);
    velocityPositionOrigin_ = dmove;
    velocityLookingAtOrigin_ = dmove;
    UpdateOrigin(currentTime);
}

void Camera::MovingUpDown(float currentTime)
{
    if (currentTime - timeOriginPosition_ > 0.1) {
        UpdateOrigin(currentTime);
    }
}

void Camera::LookOnTheLeft(float currentTime)
{
    vec3 cameraPositionToLookingAt = lookingAt_ - position_;
    vec3 horizontalAxis = normalize(cross(vec3(0.0f, 0.0f, 1.0f), cameraPositionToLookingAt));
    velocityLookingAtOrigin_ = mat3(MOVE_YAW_FACTOR) * horizontalAxis;
    UpdateOrigin(currentTime);
}

void Camera::LookingOnTheLeft(float currentTime)
{
    if (currentTime - timeOriginPosition_ > 0.1) {
        UpdateOrigin(currentTime);
    }
}

void Camera::LookOnTheRight(float currentTime)
{
    vec3 cameraPositionToLookingAt = lookingAt_ - position_;
    vec3 horizontalAxis = normalize(cross(cameraPositionToLookingAt, vec3(0.0f, 0.0f, 1.0f)));
    velocityLookingAtOrigin_ = mat3(MOVE_YAW_FACTOR) * horizontalAxis;
    UpdateOrigin(currentTime);
}

void Camera::LookingOnTheRight(float currentTime)
{
    if (currentTime - timeOriginPosition_ > 0.1) {
        UpdateOrigin(currentTime);
    }
}

void Camera::LookAbove(float currentTime)
{
    vec3 cameraPositionToLookingAt = normalize(lookingAt_ - position_);
    if (cameraPositionToLookingAt.z < 0.9) {
        vec3 op = vec3(0.0f, 0.0f, 1.0f);
        vec3 movingAxes = normalize(op - dot(op, cameraPositionToLookingAt) * cameraPositionToLookingAt);
        velocityLookingAtOrigin_ = mat3(MOVE_PITCH_FACTOR) * movingAxes;
        UpdateOrigin(currentTime);
    }
}

void Camera::LookingAbove(float currentTime)
{
    vec3 cameraPositionToLookingAt = normalize(lookingAt_ - position_);
    if (cameraPositionToLookingAt.z < 0.9 && currentTime - timeOriginLookingAt_ > 0.1) {
        UpdateOrigin(currentTime);
    }
}

void Camera::LookBelow(float currentTime)
{
    vec3 cameraPositionToLookingAt = normalize(lookingAt_ - position_);
    if (cameraPositionToLookingAt.z > -0.9) {
        vec3 op = vec3(0.0f, 0.0f, 1.0f);
        vec3 movingAxes = normalize(op - dot(op, cameraPositionToLookingAt) * cameraPositionToLookingAt);
        velocityLookingAtOrigin_ = mat3(-1 * MOVE_PITCH_FACTOR) * movingAxes;
        UpdateOrigin(currentTime);
    }
}

void Camera::LookingBelow(float currentTime)
{
    vec3 cameraPositionToLookingAt = normalize(lookingAt_ - position_);
    if (cameraPositionToLookingAt.z < 0.9 && currentTime - timeOriginLookingAt_ > 0.1) {
        UpdateOrigin(currentTime);
    }
}

void Camera::SetHeight(float height)
{
    position_.z = height;
}

void Camera::InitBezier(float time) {
    float random[18];
    for (size_t i = 0; i < 18; i++) {
        random[i] = 2*(rand()/(float)RAND_MAX)-1;
    }
    vec3 posPoints[] = {position_,
                        position_ + vec3(random[0], random[1], random[2]*2),
                        position_ + vec3(random[3], random[4], random[5]*2),
                        position_ + vec3(random[6], random[7], random[8]*2)};
    vec3 lookAtPoints[] = {lookingAt_,
                           lookingAt_ + vec3(random[ 9], random[10], random[11]),
                           lookingAt_ + vec3(random[12], random[13], random[14]),
                           lookingAt_ + vec3(random[15], random[16], random[17])};
    Bezier::Init(posPoints, lookAtPoints, 4, rand() % 20, time);
    bezierInitialized = true;
}

void Camera::MoveBezier(float time) {
    Bezier::UpdatePosition(time);
    position_ = Bezier::Bposition;
    lookingAt_ = Bezier::BlookAt;
    SetHeight(position_.z);
    if(Bezier::Brestart) {
        InitBezier(time);
    }
}

mat4 Camera::ComputeSkyView()
{
    mat4 skyplane_model_matrix = scale(mat4(4.0f), vec3(50.0f));
    vec3 eyeToPoint = normalize(lookingAt_ - position_);
    skyplane_model_matrix[3][0] = lookingAt_.x;
    skyplane_model_matrix[3][1] = lookingAt_.y;
    skyplane_model_matrix[3][2] = position_.z + 6.4f;
    skyplane_model_matrix[3][3] = 1.0f;
    vec3 drot = normalize(cross(vec3(0.0f, 0.0f, 1.0f), eyeToPoint));
    skyplane_model_matrix = rotate(skyplane_model_matrix, 0.83f, drot);
    skyplane_model_matrix = rotate(skyplane_model_matrix, phi_, vec3(0.0f, 0.0f, 1.0f));
    return skyplane_model_matrix;
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

void Camera::CorrectSphericalCoordinates()
{
    vec3 op = lookingAt_ - position_;
    op.z = 0.0f;
    vec3 opN = normalize(op);
    float costheta = dot(vec3(0.0, 0.0, 1.0), opN);
    float cosphi = dot(vec3(-1.0, 0.0, 0.0), opN);
    if (op.y < 0) {
        cosphi *= -1;
    }
    theta_ = acos(costheta);
    phi_ = acos(cosphi);
}

void Camera::CorrectLookingAt()
{
    vec3 eyeToPoint = lookingAt_ - position_;
    lookingAt_ = position_ + mat3(2.0f) * normalize(eyeToPoint);
}

void Camera::UpdateOrigin(float currentTime)
{
    timeOriginPosition_ = currentTime;
    timeOriginLookingAt_ = currentTime;
    positionOrigin_ = position_;
    lookingAtOrigin_ = lookingAt_;
    SetHeight(position_.z);
}
