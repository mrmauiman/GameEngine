#ifndef SRC_RIGIDBODY_H_
#define SRC_RIGIDBODY_H_

/*
 * Copyright 2020 Maui Kelley
 */

#include "src/gameobject.h"
#include "src/model.h"
#include "src/helper.h"

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

#endif  // SRC_RIGIDBODY_H_
