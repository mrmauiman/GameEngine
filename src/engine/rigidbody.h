#ifndef SRC_ENGINE_RIGIDBODY_H_
#define SRC_ENGINE_RIGIDBODY_H_

/*
 * Copyright 2020 Maui Kelley
 */

#include <vector>

#include "lib/glm/gtc/type_ptr.hpp"
#include "lib/glm/gtx/quaternion.hpp"
#include "lib/glm/vec3.hpp"
#include "lib/glm/vec4.hpp"

#include "src/engine/gameobject.h"
#include "src/engine/model.h"
#include "src/engine/helper.h"

namespace engine {

class RigidBody : public GameObject {
 private:
  const Model *model;
  glm::vec4 color = {1, 1, 1, 1};

 public:
  // Constructor
  // model is a pointer to a model
  explicit RigidBody(const Model *model);

  // draws the rigid body’s model with it’s current position and orientation.
  void draw() const;

  // color is a vector of size 4 representing rgba
  // sets the color member data to color
  void setColor(glm::vec4 color);

  // r, g, b, a represent a color
  // sets the color member data
  void setColor(float r, float g, float b, float a);

  // delta is the fraction of a second a frame takes
  // draws the rigidbody
  void update(float delta);
};

}  // namespace engine

#endif  // SRC_ENGINE_RIGIDBODY_H_
