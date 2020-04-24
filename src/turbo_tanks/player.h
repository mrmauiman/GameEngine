#ifndef SRC_TURBO_TANKS_PLAYER_H_
#define SRC_TURBO_TANKS_PLAYER_H_

/*
 * Copyright 2020 Maui Kelley
 */

// C/C++ standard library
#include <iostream>
#include <vector>
#include <string>
// Lib
#include "glm/vec2.hpp"
#include "glm/vec3.hpp"
// Src
#include "engine/rigid_body.h"
#include "engine/project.h"
#include "engine/camera.h"
#include "engine/animation.h"

namespace turbotanks {

class Player : public engine::RigidBody {
 private:
  glm::vec2 velocity;
  const float acceleration = 1;
  const float max_speed = 10;
  const float min_speed = 0.5f;
  const float friction = 0.8;
  const float aim_sensitivity = 5;
  const float max_energy = 100;
  const float max_health = 100;
  float energy;
  float health;
  engine::Animation move;

 public:
  // model is a pointer to a Model
  // This calls the constructor of RigidBody and also does what needs to be done
  // on creation
  explicit Player(const engine::Model* model);

  // energy is a float
  // adds energy to this->energy
  void AddEnergy(float energy) {
    this->energy = engine::clamp(this->energy + energy, 0, max_energy);
  }

  // amount is a float
  // returns if energy is greater or equal to amount
  bool HasEnergy(float amount) {
    return (amount <= energy);
  }

  float GetEnergyFraction() {
    float frac = energy/max_energy;
    return frac;
  }

  // energy is a float
  // adds energy to this->energy
  void AddHealth(float health) {
    this->health = engine::clamp(this->health + health, 0, max_health);
  }

  // amount is a float
  // returns if energy is greater or equal to amount
  bool HasHealth() {
    return (health > 0);
  }

  float GetHealthFraction() {
    return health/max_health;
  }

  void Hurt(float amount) {
    health -= amount;
    if (!HasHealth()) {
      // Kill player
    }
  }

  // delta is the time the last frame took to process
  // This function happens every frame
  void Update(float delta);
};

}  // namespace turbotanks

#endif  // SRC_TURBO_TANKS_PLAYER_H_
