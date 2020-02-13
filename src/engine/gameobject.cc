/*
 * Copyright 2020 Maui Kelley
 */

#include "src/engine/gameobject.h"

namespace engine {

  // PROTECTED

  // angle and axis describe an angle axis and radians is whether angle is
  // radians, by default this is true
  // returns a quaternion that represents the axis angle
  glm::quat GameObject::axisToQuat(float angle, glm::vec3 axis, bool radians) {
    if (!radians) {
      angle = deg2rad(angle);
    }

    return angleAxis(angle, axis);
  }

  // PUBLIC

  // Default Constructor
  GameObject::GameObject() {
    position = {0.0f, 0.0f, 0.0f};
    scale = {1.0f, 1.0f, 1.0f};
    glm::vec3 axis = {0.0f, 0.0f, -1.0f};
    orientation = glm::angleAxis(0.0f, axis);
  }

  // position is the new position
  // sets this.position to position
  void GameObject::setPosition(glm::vec3 position) {
    this->position = position;
  }

  // orientation is the new orientation
  // sets this.orientation to orientation
  void GameObject::setOrientation(glm::quat orientation) {
    this->orientation = orientation;
  }

  // angle and axis describe an angle axis and radians is whether angle is
  // radians, by default this is true
  // sets this.orientation to be a quaternion representing the angle axis
  void GameObject::setOrientation(float angle, glm::vec3 axis, bool radians) {
    orientation = axisToQuat(angle, axis, radians);
  }

  // scale is the new scale
  // sets this.scale to scale
  void GameObject::setScale(glm::vec3 scale) {
    this->scale = scale;
  }

  // returns the position
  glm::vec3 GameObject::getPosition() {
    return position;
  }

  // returns the orientation
  glm::quat GameObject::getOrientation() {
    return orientation;
  }

  // returns the scale
  glm::vec3 GameObject::getScale() {
    return scale;
  }

  // changes the game object’s current position by moving it relative to its
  // current position and orientation by the specified vector
  void GameObject::move(glm::vec3 distance) {
    position = position + (distance * orientation);
  }

  // changes the game object’s current orientation by turning it relative to
  // its current orientation by the specified angle around the specified vector
  void GameObject::turn(GLfloat angle, glm::vec3 axis, bool radians) {
    glm::quat rotate_by = axisToQuat(angle, axis, radians);
    orientation = orientation * rotate_by;
  }

  // changes the game object’s current position and orientation. The position
  // should be set to the specified eye position. The orientation should be set
  // so that it “looks at” the specified center position with the specified up
  // vector. A game object “looks at” a point by orienting so that the negative
  // z-axis points from the eye position to the center position.
  void GameObject::lookAt(glm::vec3 eye, glm::vec3 center, glm::vec3 up) {
    glm::mat4 rot_mat = glm::lookAt(eye, center, up);
    orientation = glm::quat_cast(rot_mat);
  }

}  // namespace engine
