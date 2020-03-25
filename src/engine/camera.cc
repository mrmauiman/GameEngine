/*
 * Copyright 2020 Maui Kelley
 */

#include "engine/camera.h"

namespace engine {
// PUBLIC

// fov is the field of view angle, z_near is the closest objects will be
// rendered, and z_far is the render distance.  All parameters are positive
// Creates a new camera with a frustum defined by the parameters
Camera::Camera(GLfloat fov, GLfloat z_near, GLfloat z_far, bool radians) {
  if (radians) {
    fov = rad2deg(fov);
  }
  this->fov = clamp(fov, MIN_FRUSTUM_ANGLE, MAX_FRUSTUM_ANGEL);
  this->z_near = z_near;
  this->z_far = z_far;
}

// changes the camera’s field of view by angle.
void Camera::Zoom(GLfloat fov, bool radians) {
  if (radians) {
    fov = rad2deg(fov);
  }
  this->fov = clamp(this->fov + fov, MIN_FRUSTUM_ANGLE, MAX_FRUSTUM_ANGEL);
}

// multiplies the camera’s projection matrix, calcluated using it’s current
// view frustum and the specified window width and height, with the current
// matrix.
void Camera::MultProjectionMatrix(int width, int height) const {
  // Calculate how big the frustum near is with a field of view of fov
  float frust_size = tanf(deg2rad(fov/2.0f)) * z_near;
  float ratio = width/static_cast<float>(height);

  // Create the frustum
  glFrustum(-ratio*frust_size, ratio*frust_size, -frust_size, frust_size,
            z_near, z_far);
}

// multiplies the camera’s current view matrix, calculated usings it’s
// current position and orientation, with the current matrix.
void Camera::MultViewMatrix() const {
  glm::vec3 axis = glm::axis(orientation);
  glm::mat4 rot_mat = glm::toMat4(glm::inverse(orientation));
  axis = glm::axis(glm::inverse(orientation));
  glMultMatrixf(value_ptr(rot_mat));
  glTranslatef(-position.x, -position.y, -position.z);
}

// delta is the fraction of a second a frame takes
void Camera::Update(float delta) {
  // Nothing to do for now
}
}  // namespace engine
