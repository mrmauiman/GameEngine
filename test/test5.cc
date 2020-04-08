/*
 * Copyright 2020 Maui Kelley
 */

#include <iostream>
#include <cassert>

#include "engine/model.h"
#include "engine/rigid_body.h"

int main() {
  engine::Model model("data/cube.obj");
  engine::RigidBody rigidBody1(&model);
  engine::RigidBody rigidBody2(&model);
  glm::vec3 point_in_model(0.0f, 0.0f, 0.0f);
  glm::vec3 point_not_in_model(0.0f, 0.0f, 100.0f);
  rigidBody1.Intersects(rigidBody2);
  exit(EXIT_SUCCESS);
}
