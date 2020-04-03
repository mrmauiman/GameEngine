#ifndef SRC_ENGINE_RIGID_BODY_H_
#define SRC_ENGINE_RIGID_BODY_H_

/*
 * Copyright 2020 Maui Kelley
 */

#include <vector>

#include "glm/gtc/type_ptr.hpp"
#include "glm/gtx/quaternion.hpp"
#include "glm/vec3.hpp"
#include "glm/vec4.hpp"

#include "engine/game_object.h"
#include "engine/model.h"
#include "engine/helper.h"

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
  void Draw() const;

  // color is a vector of size 4 representing rgba
  // sets the color member data to color
  void SetColor(glm::vec4 color);

  // r, g, b, a represent a color
  // sets the color member data
  void SetColor(float r, float g, float b, float a);

  // delta is the fraction of a second a frame takes
  // draws the rigid_body
  void Update(float delta);
};

}  // namespace engine

#endif  // SRC_ENGINE_RIGID_BODY_H_
