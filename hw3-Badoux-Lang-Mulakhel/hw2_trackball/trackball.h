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
      float angle = acos(dot(current_pos, anchor_pos_) / (length(current_pos) * length(anchor_pos_)));
      // Rodrigues’ Rotation Formula
      mat3 k = mat3(1.0f);
      k[0][0] = 0;
      k[0][1] = axis[2];
      k[0][2] = -axis[1];
      k[1][0] = -axis[2];
      k[1][1] = 0;
      k[1][2] = axis[0];
      k[2][0] = axis[1];
      k[2][1] = -axis[0];
      k[2][2] = 0;
      mat3 rotationMatrix = mat3(1.0f) + sin(angle) * k + (1 - cos(angle)) * k * k;
      mat4 rotationMatrix4 = mat4(rotationMatrix);
      return rotationMatrix4;
    }

private:
    // projects the point p (whose z coordiante is still empty/zero) onto the
    // trackball surface. If the position at the mouse cursor is outside the
    // trackball, use a hyberbolic sheet as explained in:
    // https://www.opengl.org/wiki/Object_Mouse_Trackball.
    // The trackball radius is given by 'radius_'.
    void ProjectOntoSurface(vec3& p) const {
        // TODO 2: Implement this function. Read above link for details.
        float pxSquare = p.x * p.x;
        float pySquare = p.y * p.y;
        float radiusSquare = radius_ * radius_;
        if ((pxSquare + pySquare) > radiusSquare) {
          p.z = radiusSquare / (2 * sqrt(pxSquare + pySquare));
      } else {
          p.z = sqrt(radiusSquare - pxSquare - pySquare);
      }
    }

    float radius_;
    vec3 anchor_pos_;
    mat4 rotation_;
};
