#ifndef SRC_ENGINE_RIGIDBODY_H_
#define SRC_ENGINE_RIGIDBODY_H_

/*
 * Copyright 2020 Maui Kelley
 */

#include "lib/glm/gtc/type_ptr.hpp"
#include "lib/glm/gtx/quaternion.hpp"
#include "lib/glm/vec3.hpp"

#include "src/engine/gameobject.h"
#include "src/engine/model.h"
#include "src/engine/helper.h"

namespace engine {

class RigidBody : public GameObject {
 private:
  const Model *model;

 public:
  // Constructor
  // model is a pointer to a model
  explicit RigidBody(const Model *model);

  // draws the rigid body’s model with it’s current position and orientation.
  void draw() const;

  // delta is the fraction of a second a frame takes
  // draws the rigidbody
  void update(float delta);
};

}  // namespace engine

#endif  // SRC_ENGINE_RIGIDBODY_H_
