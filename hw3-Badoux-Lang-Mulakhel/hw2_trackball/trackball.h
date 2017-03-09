#pragma once
#include "icg_helper.h"


using namespace glm;

class Trackball {
public:
    Trackball() : radius_(1.0f) {}

    // this function is called when the user presses the left mouse button down.
    // x, and y are in [-1, 1]. (-1, -1) is the bottom left corner while (1, 1)
    // is the top right corner.
    void BeingDrag(float x, float y) {
      anchor_pos_ = vec3(x, y, 0.0f);
      ProjectOntoSurface(anchor_pos_);
    }

    // this function is called while the user moves the curser around while the
    // left mouse button is still pressed.
    // x, and y are in [-1, 1]. (-1, -1) is the bottom left corner while (1, 1)
    // is the top right corner.
    // returns the rotation of the trackball in matrix form.
    mat4 Drag(float x, float y) {
      vec3 current_pos = vec3(x, y, 0.0f);
      ProjectOntoSurface(current_pos);

      mat4 rotation = IDENTITY_MATRIX;
      // TODO 3: Calculate the rotation given the projections of the anocher
      // point and the current position. The rotation axis is given by the cross
      // product of the two projected points, and the angle between them can be
      // used as the magnitude of the rotation.
      // you might want to scale the rotation magnitude by a scalar factor.
      // p.s. No need for using complicated quaternions as suggested inthe wiki
      // article.
      vec3 axis = cross(current_pos, anchor_pos_);
      float angle = acos(dot(current_pos, anchor_pos_));
      // Rodrigues’ Rotation Formula
      mat3 mat = mat3(1.0f);
      mat[0][0] = 0;
      mat[0][1] = -axis[2];
      mat[0][2] = axis[1];
      mat[1][0] = axis[2];
      mat[1][1] = 0;
      mat[1][2] = -axis[0];
      mat[2][0] = -axis[1];
      mat[2][1] = axis[0];
      mat[2][2] = 0;
      mat3 mat2 = cos(angle) * mat3(1.0f) + (1 - cos(angle)) * mat3(axis * axis[0], axis * axis[1], axis * axis[2]) + sin(angle) * mat;
      rotation = mat4(mat2);
      return rotation;
    }

private:
    // projects the point p (whose z coordiante is still empty/zero) onto the
    // trackball surface. If the position at the mouse cursor is outside the
    // trackball, use a hyberbolic sheet as explained in:
    // https://www.opengl.org/wiki/Object_Mouse_Trackball.
    // The trackball radius is given by 'radius_'.
    void ProjectOntoSurface(vec3& p) const {
      // TODO 2: Implement this function. Read above link for details.
      if(pow(p.x,2.0) + pow(p.y, 2.0) > pow(radius_, 2.0)){
          p.z = pow(radius_, 2.0)/sqrt(pow(p.x,2.0) + pow(p.y, 2.0));
      } else {
          p.z = sqrt(pow(radius_, 2.0) - pow(p.x,2.0) + pow(p.y, 2.0));
      }
    }

    float radius_;
    vec3 anchor_pos_;
    mat4 rotation_;
};
