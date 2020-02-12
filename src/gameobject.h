#ifndef SRC_GAMEOBJECT_H_
#define SRC_GAMEOBJECT_H_

/*
 * Copyright 2020 Maui Kelley
 */

#include <math.h>

#include "lib/glm/gtc/quaternion.hpp"
#include "lib/glm/vec3.hpp"
#include "src/constants.h"

namespace engine {

class GameObject {
 protected:
  glm::vec3 position;
  glm::quat orientation;

  // angle and axis describe an angle axis and radians is whether angle is
  // radians, by default this is true
  // returns a quaternion that represents the axis angle
  glm::quat axisToQuat(float angle, glm::vec3 axis, bool radians);

 public:
  // Default Constructor
  GameObject();

  // position is the new position
  // sets this.position to position
  void setPosition(glm::vec3 position);

  // orientation is the new orientation
  // sets this.orientation to orientation
  void setOrientation(glm::quat orientation);

  // angle and axis describe an angle axis and radians is whether angle is
  // radians, by default this is true
  // sets this.orientation to be a quaternion representing the angle axis
  void setOrientation(float angle, glm::vec3 axis, bool radians = true);

  // returns the position
  glm::vec3 getPosition();

  // returns the orientation
  glm::vec3 getOrientation();

  // changes the game object’s current position by moving it relative to its
  // current position and orientation by the specified vector
  void move(glm::vec3 distance);

  // changes the game object’s current orientation by turning it relative to
  // its current orientation by the specified angle around the specified vector
  void turn(GLfloat angle, glm::vec3 axis, bool radians = true);

  // changes the game object’s current position and orientation. The position
  // should be set to the specified eye position. The orientation should be set
  // so that it “looks at” the specified center position with the specified up
  // vector. A game object “looks at” a point by orienting so that the negative
  // z-axis points from the eye position to the center position.
  void LookAt(glm::vec3 eye, glm::vec3 center, glm::vec3 up);

  // Virtual Function Update
  virtual void update(float delta);
}
}  // namespace engine

#endif  // SRC_GAMEOBJECT_H_
