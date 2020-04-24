#ifndef SRC_TURBO_TANKS_PLAYER_CANNON_H_
#define SRC_TURBO_TANKS_PLAYER_CANNON_H_

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
#include "turbo_tanks/player.h"
#include "turbo_tanks/energy_ball.h"
#include "turbo_tanks/player_camera.h"

namespace turbotanks {

class Player;
class PlayerCannon : public engine::RigidBody {
 private:
  float cooldown;
  const float machinegun_cooldown = 0.1f;  // seconds
  const float machinegun_bullet_speed = 15;
  const float machine_cost = 0.1;
  const engine::Model* energyball_md;
 public:
  Player* player;
  engine::Camera* camera;

  // Constructor
  explicit PlayerCannon(const engine::Model* model, const engine::Model* eb_md):
  engine::RigidBody(model) {
    cooldown = 0;
    energyball_md = eb_md;
    tags.push_back("playercannon");
  }

  // Override parent Update
  void Update(float delta);
};

}  // namespace turbotanks

#endif  // SRC_TURBO_TANKS_PLAYER_CANNON_H_
