#ifndef SRC_ENGINE_GAMEOBJECT_H_
#define SRC_ENGINE_GAMEOBJECT_H_

/*
 * Copyright 2020 Maui Kelley
 */

#include <math.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include "glm/gtx/quaternion.hpp"
#include "glm/vec3.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/mat4x4.hpp"
#include "engine/constants.h"
#include "engine/helper.h"

namespace engine {

class GameObject {
 protected:
  glm::vec3 position;
  glm::vec3 scale;
  glm::quat orientation;

  // angle and axis describe an angle axis and radians is whether angle is
  // radians, by default this is true
  // returns a quaternion that represents the axis angle
  glm::quat AxisToQuat(float angle, glm::vec3 axis, bool radians);

 public:
  // Default Constructor
  GameObject();

  // position is the new position
  // sets this.position to position
  void SetPosition(glm::vec3 position);

  // orientation is the new orientation
  // sets this.orientation to orientation
  void SetOrientation(glm::quat orientation);

  // angle and axis describe an angle axis and radians is whether angle is
  // radians, by default this is true
  // sets this.orientation to be a quaternion representing the angle axis
  void SetOrientation(float angle, glm::vec3 axis, bool radians = false);

  // scale is the new scale
  // sets this.scale to scale
  void SetScale(glm::vec3 scale);

  // returns the position
  glm::vec3 GetPosition();

  // returns the orientation
  glm::quat GetOrientation();

  // returns the scale
  glm::vec3 GetScale();

  // changes the game object’s current position by moving it relative to its
  // current position and orientation by the specified vector
  void Move(glm::vec3 distance);

  // changes the game object’s current orientation by turning it relative to
  // its current orientation by the specified angle around the specified vector
  void Turn(GLfloat angle, glm::vec3 axis, bool radians = false);

  // changes the game object’s current position and orientation. The position
  // should be set to the specified eye position. The orientation should be set
  // so that it “looks at” the specified center position with the specified up
  // vector. A game object “looks at” a point by orienting so that the negative
  // z-axis points from the eye position to the center position.
  void LookAt(glm::vec3 eye, glm::vec3 center, glm::vec3 up);

  // Virtual Function Update
  virtual void Update(float delta) = 0;
};
}  // namespace engine

#endif  // SRC_ENGINE_GAMEOBJECT_H_
