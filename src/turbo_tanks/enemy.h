#ifndef SRC_TURBO_TANKS_ENEMY_H_
#define SRC_TURBO_TANKS_ENEMY_H_

/*
 * Copyright 2020 Maui Kelley
 */

// C/C++ standard library
#include <math.h>
#include <iostream>
#include <cmath>
#include <string>
#include <vector>
// Lib
#include "glm/vec3.hpp"
#include "glm/vec2.hpp"
// Src
#include "engine/model.h"
#include "engine/rigid_body.h"
#include "turbo_tanks/player.h"
#include "turbo_tanks/enemy_cannon.h"

namespace turbotanks {

class Enemy : public engine::RigidBody {
 private:
  float health;
  glm::vec3 velocity;
  const float max_health = 100;
  const float cone_angle = 20;
  const float view_dist = 20;
  const float spin_speed = 45;
  const float movespeed = 2;

 public:
  EnemyCannon* cannon;
  Player* player;

  explicit Enemy(const engine::Model* model) : engine::RigidBody(model) {
    health = max_health;
    velocity = glm::vec3(0, 0, 0);
    tags.push_back("enemy");
  }

  void Hurt(float amount) {
    health -= amount;
    if (health < 0) {
      // std::cout << id << std::endl;
      project->RemoveRigidBody(cannon->id);
      project->RemoveRigidBody(id);
    }
  }

  void Update(float delta) {
    glm::vec3 dif = player->GetPosition() - GetPosition();
    float len = glm::length(dif);
    dif = glm::normalize(dif);
    glm::vec3 z_vec = glm::rotate(GetOrientation(), glm::vec3(0, 0, -1));
    z_vec = glm::normalize(z_vec);
    float dot = glm::dot(glm::vec2(z_vec.x, z_vec.z), glm::vec2(dif.x, dif.z));
    float angle = engine::rad2deg(acos(dot));
    std::vector<std::string> ignore = {"enemy", "energyball", "player",
    "playercannon", "enemycannon"};
    if (std::abs(angle) <= cone_angle && len < view_dist &&
    project->RayCast(GetPosition(), player->GetPosition(), ignore) == -1) {
      LookAt(GetPosition(), player->GetPosition(), glm::vec3(0, 1, 0));
      cannon->can_see = true;
      if (len > view_dist/2.0f) {
        velocity.z = engine::clamp(velocity.z-(movespeed*delta),
        -movespeed, movespeed);
      } else {
        velocity.z = 0;
      }
    } else {
      cannon->can_see = false;
      Turn(spin_speed*delta, glm::vec3(0, 1, 0));
    }

    // Collision
    Move(velocity);
    ignore = {"camera", "energyball", "floor", "collectable", "enemycannon"};
    if (project->Collides(id, ignore) != -1) {
      Move(-velocity);
      velocity.z = -velocity.z/2.0f;
    }

    RigidBody::Update(delta);
  }
};

}  // namespace turbotanks

#endif  // SRC_TURBO_TANKS_ENEMY_H_
