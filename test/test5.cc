/*
 * Copyright 2020 Maui Kelley
 */

#include <iostream>
#include <cassert>

#include "engine/model.h"
#include "engine/rigid_body.h"

int main() {
  engine::Model model("data/model.obj");
  engine::RigidBody rigidBody(&model);
  glm::vec3 point_in_model(0.0f, 0.0f, 0.0f);
  glm::vec3 point_not_in_model(0.0f, 0.0f, 100.0f);
  assert(rigidBody.Intersects(point_in_model));
  assert(!rigidBody.Intersects(point_not_in_model);
  std::cout << "All assertions passed!" << std::endl;
  exit(EXIT_SUCCESS);
}
