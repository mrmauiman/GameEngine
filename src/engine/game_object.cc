/*
 * Copyright 2020 Maui Kelley
 */

#include "engine/game_object.h"

namespace engine {

  // PROTECTED

  // PUBLIC

  // Default Constructor
  GameObject::GameObject() {
    position = {0.0f, 0.0f, 0.0f};
    scale = {1.0f, 1.0f, 1.0f};
    glm::vec3 axis = {0.0f, 0.0f, -1.0f};
    orientation = glm::angleAxis(0.0f, axis);
    SetBoundingBox(glm::vec3(0, 0, 0), glm::vec3(0, 0, 0));
    bounding_box_axis_aligned = false;
  }

  // tag is a string
  // returns if tag is in tags
  bool GameObject::HasTag(std::string tag) {
    bool found = false;
    int i = 0;
    while (i < tags.size() && !found) {
      if (tags[i] == tag) {
        found = true;
      }
      i++;
    }
    return found;
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
      glm::vec3 temp = points[i];
      if (!bounding_box_axis_aligned) {
        temp = glm::rotate(orientation, temp);
      }
      temp += position;
      rv[i] = temp;
    }

    return rv;
  }

  // point is a global position
  // returns true if point is in or touching the bounding box of this and
  // false otherwise
  bool GameObject::Intersects(glm::vec3 point) const {
    point -= position;
    point = glm::rotate(glm::inverse(orientation), point);
    return (point.x >= bounding_box_min.x && point.x <= bounding_box_max.x &&
            point.y >= bounding_box_min.y && point.y <= bounding_box_max.y &&
            point.z >= bounding_box_min.z && point.z <= bounding_box_max.z);
  }

  // other is another gameobject
  // returns true if the other gameobject is colliding with this and false
  // otherwise
  bool GameObject::Intersects(const GameObject & other) const {
    bool rv = true;
    glm::vec3 * my_points = GetBoundingBoxPoints();
    glm::vec3 * other_points = other.GetBoundingBoxPoints();
    glm::vec3 axises[NUM_BOX_AXIS] = {
      glm::normalize(my_points[0] - my_points[1]),
      glm::normalize(my_points[0] - my_points[2]),
      glm::normalize(my_points[0] - my_points[3]),
      glm::normalize(other_points[0] - other_points[1]),
      glm::normalize(other_points[0] - other_points[2]),
      glm::normalize(other_points[0] - other_points[3])
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

  // start and end define a line segment in 3d space
  // returns the first position on that segment that this intersects or -1 if
  // this doesn't intersect
  float GameObject::RayCast(glm::vec3 start, glm::vec3 end) const {
    float rv = -1;
    // Create points and faces for bounding box
    glm::vec3* points = GetBoundingBoxPoints();
    int faces[6][4] = {
      {0, 1, 3, 6},
      {0, 3, 5, 2},
      {2, 5, 4, 7},
      {1, 6, 4, 7},
      {0, 2, 1, 7},
      {3, 5, 4, 6}
    };

    // get the first intersection
    for (int i = 0; i < 6; i++) {
      float temp = GetCollisionOnLine(start, end, faces[i], points);
      if (temp != -1) {
        if (rv == -1 || temp < rv) {
          rv = temp;
        }
      }
    }

    // clean up heap
    delete[] points;
    return rv;
  }

  // Overload << operator
  std::ostream& operator<<(std::ostream& os, const GameObject& go) {
    os << "[" << go.id << "]: {";
    for (int i = 0; i < go.tags.size(); i++) {
      os << "\"" << go.tags[i] << "\"";
      if (i < go.tags.size()-1) {
        os << ", ";
      }
    }
    glm::vec3 pos = go.position;
    os << "}, {p: (" << pos.x << ", " << pos.y << ", " << pos.z << "), o: {"
    << glm::angle(go.orientation) << ", (";
    glm::vec3 r_axis = glm::axis(go.orientation);
    os << r_axis.x << ", " << r_axis.y << ", " << r_axis.z << ")}, s: (" <<
    go.scale.x << ", " << go.scale.y << ", " << go.scale.z << ")}";
    return os;
  }

}  // namespace engine
