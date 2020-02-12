#ifndef SRC_CAMERA_H_
#define SRC_CAMERA_H_

/*
 * Copyright 2020 Maui Kelley
 */

#include <GLFW/glfw3.h>

#include "lib/glm/gtc/quaternion.hpp"
#include "lib/glm/vec3.hpp"
#include "src/gameobject.h"
#include "src/constants.h"
#include "src/helper.h"

namespace engine {

class Camera : public GameObject {
 private:
  GLfloat fov;
  GLfloat z_near;
  GLfloat z_far;

 public:
  // Constructor
  // fovy is the field of view, z_near is the closest objects will be rendered,
  // and z_far is the render distance.  All parameters are positive
  // Creates a new camera with a frustum defined by the parameters
  Camera(GLfloat fov, GLfloat z_near, GLfloat z_far, bool rad = true);

  // changes the camera’s field of view by angle.
  void zoom(GLfloat angle);

  // multiplies the camera’s projection matrix, calcluated using it’s current
  // view frustum and the specified window width and height, with the current
  // matrix.
  void multProjectionMatrix(int width, int height) const;

  // multiplies the camera’s current view matrix, calculated usings it’s
  // current position and orientation, with the current matrix.
  void multViewMatrix() const;

  // delta is the fraction of a second a frame takes
  // creates the frustum for display
  // sets the matrix mode to GL_PROJECTION
  void update(float delta);
};

}  // namespace engine

#endif  // SRC_CAMERA_H_
