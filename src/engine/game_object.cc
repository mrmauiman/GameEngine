/*
 * Copyright 2020 Maui Kelley
 */

#include "engine/game_object.h"

namespace engine {

  // PROTECTED

  // angle and axis describe an angle axis and radians is whether angle is
  // radians, by default this is true
  // returns a quaternion that represents the axis angle
  glm::quat GameObject::AxisToQuat(float angle, glm::vec3 axis, bool radians)
    const {
    if (!radians) {
      angle = deg2rad(angle);
    }

    return glm::angleAxis(angle, axis);
  }

  // axis is an axis in 3d space and point is a point in 3d space
  // returns the value of how far along the axis point is
  float GameObject::ValueOnAxis(glm::vec3 axis, glm::vec3 point) const {
    axis = glm::normalize(axis);
    glm::vec3 x_axis(1, 0, 0);
    glm::vec3 r_axis = glm::normalize(glm::cross(axis, x_axis));
    float angle = glm::angle(axis, x_axis);
    point = glm::rotate(AxisToQuat(angle, r_axis, true), point);
    return point.x;
  }

  // min_max is a array of size 2, axis is an axis in 3D space, points is an
  // array of 8 points
  // sets min_max[0] to the smallest value on axis in points and min_max[1] to
  // the largest
  void GameObject::GetMinMaxOnAxis(float * min_max, glm::vec3 axis,
                                   glm::vec3 * points) const {
    min_max[0] = ValueOnAxis(axis, points[0]);
    min_max[1] = ValueOnAxis(axis, points[0]);
    for (int i = 1; i < NUM_BOX_POINTS; i++) {
      float val = ValueOnAxis(axis, points[i]);
      min_max[0] = (min_max[0] > val)?val:min_max[0];
      min_max[1] = (min_max[1] < val)?val:min_max[1];
    }
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

  // returns an array of size 8 that represents all vertices in our bounding box
  glm::vec3 * GameObject::GetBoundingBoxPoints() const {
    glm::vec3 * rv = new glm::vec3[NUM_BOX_POINTS];
    glm::vec3 points[NUM_BOX_POINTS] = {
      bounding_box_min,
      glm::vec3(bounding_box_max.x, bounding_box_min.y, bounding_box_min.z),
      glm::vec3(bounding_box_min.x, bounding_box_min.y, bounding_box_max.z),
      glm::vec3(bounding_box_min.x, bounding_box_max.y, bounding_box_min.z),
      bounding_box_max,
      glm::vec3(bounding_box_min.x, bounding_box_max.y, bounding_box_max.z),
      glm::vec3(bounding_box_max.x, bounding_box_max.y, bounding_box_min.z),
      glm::vec3(bounding_box_max.x, bounding_box_min.y, bounding_box_max.z)
    };

    for (int i = 0; i < NUM_BOX_POINTS; i++) {
      glm::vec3 temp = glm::rotate(orientation, points[i]);
      temp += position;
      rv[i] = temp;
    }

    return rv;
  }

  // other is another gameobject
  // returns true if the other gameobject is colliding with this and false
  // otherwise
  bool GameObject::Intersects(const GameObject & other) const {
    bool rv = true;
    glm::vec3 * my_points = GetBoundingBoxPoints();
    glm::vec3 * other_points = other.GetBoundingBoxPoints();
    glm::vec3 axises[NUM_BOX_AXIS] = {
      my_points[0] - my_points[1],
      my_points[0] - my_points[2],
      my_points[0] - my_points[3],
      other_points[0] - other_points[1],
      other_points[0] - other_points[2],
      other_points[0] - other_points[3],
    };

    for (int i = 0; i < NUM_BOX_AXIS && rv; i++) {
      float my_min_max[2] = {0, 0};
      float other_min_max[2] = {0, 0};

      GetMinMaxOnAxis(my_min_max, axises[i], my_points);
      GetMinMaxOnAxis(other_min_max, axises[i], other_points);

      if ((my_min_max[0] > other_min_max[1] && my_min_max[0] > other_min_max[0])
      || (my_min_max[1] < other_min_max[1] && my_min_max[1] < other_min_max[0])
      ) {
        rv = false;
      }
    }

    delete[] my_points;
    delete[] other_points;
    return rv;
  }

}  // namespace engine
