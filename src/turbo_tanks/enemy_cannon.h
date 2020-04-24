#ifndef SRC_TURBO_TANKS_ENEMY_CANNON_H_
#define SRC_TURBO_TANKS_ENEMY_CANNON_H_

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
#include "glm/vec4.hpp"
// Src
#include "engine/rigid_body.h"
#include "turbo_tanks/energy_ball.h"
#include "turbo_tanks/player.h"

namespace turbotanks {

class EnemyCannon : public engine::RigidBody {
 private:
  float cooldown;
  const float machinegun_cooldown = 0.1f;  // seconds
  const float machinegun_bullet_speed = 15;
  const float machine_cost = 0.1;
  const engine::Model* energyball_md;

 public:
  RigidBody* enemy;
  Player* player;
  bool can_see;

  // Constructor
  explicit EnemyCannon(const engine::Model* model, const engine::Model* eb_md):
  engine::RigidBody(model) {
    cooldown = 0;
    energyball_md = eb_md;
    tags.push_back("enemycannon");
    can_see = false;
  }

  // Override parent Update
  void Update(float delta) {
    if (can_see) {
      LookAt(enemy->GetPosition(), player->GetPosition(), glm::vec3(0, 1, 0));

      // Fireing
      if (cooldown <= 0) {
        // Machine Gun
        glm::quat dir = GetOrientation();
        glm::vec3 bullet_v = glm::vec3(0, 0, -1);
        bullet_v *= (machinegun_bullet_speed*delta);
        EnergyBall* bullet = new EnergyBall(bullet_v, energyball_md);
        bullet->cannon = this;
        bullet->parent = enemy;
        bullet->ignore =  {"enemy", "enemycannon"};
        bullet->SetColor(glm::vec4(1, 0, 0, 1));
        bullet->SetPosition(bullet->parent->GetPosition());
        bullet->SetOrientation(bullet->cannon->GetOrientation());
        // std::cout << "here" << std::endl;
        project->AddRigidBody(bullet);
        cooldown = machinegun_cooldown;
      } else {
        cooldown -= delta;
      }
    } else {
      SetPosition(enemy->GetPosition());
      SetOrientation(enemy->GetOrientation());
    }

    RigidBody::Update(delta);
  }
};

}  // namespace turbotanks

#endif  // SRC_TURBO_TANKS_ENEMY_CANNON_H_
