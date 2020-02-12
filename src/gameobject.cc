/*
 * Copyright 2020 Maui Kelley
 */

#include "src/gameobject.h"

namespace engine {

  // PROTECTED

  // angle and axis describe an angle axis and radians is whether angle is
  // radians, by default this is true
  // returns a quaternion that represents the axis angle
  glm::quat GameObject::axisToQuat(float angle, glm::vec3 axis, bool radians) {
    if (!radians) {
      angle = (angle/180)*PI;
    }

    return angleAxis(angle, axis);
  }

  // PUBLIC

  // Default Constructor
  GameObject::GameObject() {
    position = {0.0f, 0.0f, 0.0f};
    orientation = glm::angleAxis(0.0f, 0.0f, 0.0f, 0.0f);
  }

  // position is the new position
  // sets this.position to position
  void GameObject::setPosition(glm::vec3 position) {
    this.position = position;
  }

  // orientation is the new orientation
  // sets this.orientation to orientation
  void GameObject::setOrientation(glm::quat orientation) {
    this.orientation = orientation;
  }

  // angle and axis describe an angle axis and radians is whether angle is
  // radians, by default this is true
  // sets this.orientation to be a quaternion representing the angle axis
  void GameObject::setOrientation(float angle, glm::vec3 axis,
                                  bool radians = true) {
    orientation = axisToQuat(angle, axis, radians);
  }

  // returns the position
  glm::vec3 GameObject::getPosition() {
    return position;
  }

  // returns the orientation
  glm::vec3 GameObject::getOrientation() {
    return position;
  }

  // changes the game object’s current position by moving it relative to its
  // current position and orientation by the specified vector
  void GameObject::move(glm::vec3 distance) {
    position = position + (distance * orientation);
  }

  // changes the game object’s current orientation by turning it relative to
  // its current orientation by the specified angle around the specified vector
  void GameObject::turn(GLfloat angle, glm::vec3 axis, bool radians = true) {
    glm::rotate_by = axisToQuat(angle, axis, radians);

    orientation = orientation * rotate_by;
  }

  // changes the game object’s current position and orientation. The position
  // should be set to the specified eye position. The orientation should be set
  // so that it “looks at” the specified center position with the specified up
  // vector. A game object “looks at” a point by orienting so that the negative
  // z-axis points from the eye position to the center position.
  void GameObject::LookAt(glm::vec3 eye, glm::vec3 center, glm::vec3 up) {
    position = eye;
    float up_angle = atan(up.x/up.y) - (PI/2);
    orientation = axisToQuat(up_angle, position - center, true);
  }

}  // namespace engine
