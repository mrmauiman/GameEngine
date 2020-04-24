#ifndef SRC_TURBO_TANKS_ENERGY_BALL_H_
#define SRC_TURBO_TANKS_ENERGY_BALL_H_

/*
 * Copyright 2020 Maui Kelley
 */

// C/C++ standard library
#include <iostream>
#include <vector>
#include <string>
// Lib
#include "glm/vec3.hpp"
// Src
#include "engine/model.h"
#include "engine/rigid_body.h"
#include "engine/project.h"

namespace turbotanks {

class Enemy;
class Player;
class EnergyBall : public engine::RigidBody {
 private:
  glm::vec3 velocity;
  float life_timer;
  const float lifespan = 10;  // seconds

 public:
  std::vector<std::string> ignore;
  RigidBody* cannon;
  RigidBody* parent;
  // Constructor
  explicit EnergyBall(glm::vec3 velocity, const engine::Model* md) :
  engine::RigidBody(md) {
    this->velocity = velocity;
    life_timer = 0;
    tags.push_back("energyball");
  }

  // Override parent Update
  void Update(float delta);

  // Deconstructor
  ~EnergyBall() {
    delete model;
  }
};

}  // namespace turbotanks

#endif  // SRC_TURBO_TANKS_ENERGY_BALL_H_
