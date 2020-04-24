#ifndef SRC_ENGINE_GAME_OBJECT_H_
#define SRC_ENGINE_GAME_OBJECT_H_

/*
 * Copyright 2020 Maui Kelley
 */

#include <math.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>
#include <vector>

#include "glm/gtx/quaternion.hpp"
#include "glm/vec3.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/mat4x4.hpp"
#include "glm/gtx/vector_angle.hpp"

#include "engine/constants.h"
#include "engine/helper.h"

namespace engine {

// Forward declare Project
class Project;
class GameObject {
 protected:
  glm::vec3 position;
  glm::vec3 scale;
  glm::quat orientation;

  // Bounding Box
  glm::vec3 bounding_box_min, bounding_box_max;

 public:
  Project* project;
  int id;
  std::vector<std::string> tags;
  bool bounding_box_axis_aligned;

  // Default Constructor
  GameObject();

  // tag is a string
  // returns if tag is in tags
  bool HasTag(std::string tag);

  // position is the new position
  // sets this.position to position
  void SetPosition(glm::vec3 position);

  // x, y, and z describe a location
  // sets this.position to x, y, z
  void SetPosition(float x, float y, float z);

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

  // minimum and maximum define a bounding box
  // sets the bounding box memeber data
  void SetBoundingBox(glm::vec3 minimum, glm::vec3 maximum);

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

  // returns an array of size 8 that represents all vertices in our bounding box
  glm::vec3 * GetBoundingBoxPoints() const;

  // point is a global position
  // returns true if point is in or touching the bounding box of this and
  // false otherwise
  bool Intersects(glm::vec3 point) const;

  // other is another gameobject
  // returns true if the other gameobject is colliding with this and false
  // otherwise
  bool Intersects(const GameObject & other) const;

  // start and end define a line segment in 3d space
  // returns the first position on that segment that this intersects or -1 if
  // this doesn't intersect
  float RayCast(glm::vec3 start, glm::vec3 end) const;

  // Virtual Function Update
  virtual void Update(float delta) = 0;

  // Overload << operator
  friend std::ostream& operator<<(std::ostream& os, const GameObject& go);
};
}  // namespace engine

#endif  // SRC_ENGINE_GAME_OBJECT_H_
