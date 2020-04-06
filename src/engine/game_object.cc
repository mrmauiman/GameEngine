/*
 * Copyright 2020 Maui Kelley
 */

#include "engine/game_object.h"

namespace engine {

  // PROTECTED

  // angle and axis describe an angle axis and radians is whether angle is
  // radians, by default this is true
  // returns a quaternion that represents the axis angle
  glm::quat GameObject::AxisToQuat(float angle, glm::vec3 axis, bool radians) {
    if (!radians) {
      angle = deg2rad(angle);
    }

    return glm::angleAxis(angle, axis);
  }

  // PUBLIC

  // Default Constructor
  GameObject::GameObject() {
    position = {0.0f, 0.0f, 0.0f};
    scale = {1.0f, 1.0f, 1.0f};
    glm::vec3 axis = {0.0f, 0.0f, -1.0f};
    orientation = glm::angleAxis(0.0f, axis);
    SetBoundingBox(glm::vec3(0, 0, 0), glm::vec3(0, 0, 0));
  }

  // position is the new position
  // sets this.position to position
  void GameObject::SetPosition(glm::vec3 position) {
    this->position = position;
  }

  // x, y, and z describe a location
  // sets this.position to x, y, z
  void GameObject::SetPosition(float x, float y, float z) {
    SetPosition(glm::vec3(x, y, z));
  }

  // orientation is the new orientation
  // sets this.orientation to orientation
  void GameObject::SetOrientation(glm::quat orientation) {
    this->orientation = orientation;
  }

  // angle and axis describe an angle axis and radians is whether angle is
  // radians, by default this is true
  // sets this.orientation to be a quaternion representing the angle axis
  void GameObject::SetOrientation(float angle, glm::vec3 axis, bool radians) {
    SetOrientation(AxisToQuat(angle, axis, radians));
  }

  // scale is the new scale
  // sets this.scale to scale
  void GameObject::SetScale(glm::vec3 scale) {
    this->scale = scale;
  }

  // minimum and maximum define a bounding box
  // sets the bounding box memeber data
  void GameObject::SetBoundingBox(glm::vec3 minimum, glm::vec3 maximum) {
    bounding_box_min = minimum;
    bounding_box_max = maximum;
  }

  // returns the position
  glm::vec3 GameObject::GetPosition() {
    return position;
  }

  // returns the orientation
  glm::quat GameObject::GetOrientation() {
    return orientation;
  }

  // returns the scale
  glm::vec3 GameObject::GetScale() {
    return scale;
  }

  // changes the game object’s current position by moving it relative to its
  // current position and orientation by the specified vector
  void GameObject::Move(glm::vec3 distance) {
    SetPosition(position + (orientation * distance));
  }

  // changes the game object’s current orientation by turning it relative to
  // its current orientation by the specified angle around the specified vector
  void GameObject::Turn(GLfloat angle, glm::vec3 axis, bool radians) {
    glm::quat rotate_by = AxisToQuat(angle, axis, radians);
    SetOrientation(orientation * rotate_by);
  }

  // changes the game object’s current position and orientation. The position
  // should be set to the specified eye position. The orientation should be set
  // so that it “looks at” the specified center position with the specified up
  // vector. A game object “looks at” a point by orienting so that the negative
  // z-axis points from the eye position to the center position.
  void GameObject::LookAt(glm::vec3 eye, glm::vec3 center, glm::vec3 up) {
    SetPosition(eye);
    glm::mat4 rot_mat = glm::inverse(glm::lookAt(eye, center, up));
    SetOrientation(glm::quat_cast(rot_mat));
  }

  // point is a global position
  // returns true if point is in or touching the bounding box of this and
  // false otherwise
  bool GameObject::Intersects(glm::vec3 point) const {
    point -= position;
    point = glm::rotate(glm::inverse(orientation), point);
    return (point.x >= bounding_box_min.x && point.x <= bounding_box_max.x &&
            point.y >= bounding_box_min.y && point.y <= bounding_box_max.y);
  }

}  // namespace engine
