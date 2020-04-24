/*
 * Copyright 2020 Maui Kelley
 */

#include "turbo_tanks/player_cannon.h"

namespace turbotanks {

// Override parent Update
void PlayerCannon::Update(float delta) {
  bool machinegun_input = project->GetButtonInput("machinegun");

  glm::quat camera_o = camera->GetOrientation();
  glm::vec3 camera_p = camera->GetPosition();
  glm::vec3 end = camera_p + (camera_o * glm::vec3(0, 0, -50));
  std::vector<std::string> ignore = {"player", "playercannon", "camera",
  "energyball"};
  float dist = project->RayCast(camera_p, end, ignore);
  glm::vec3 target;
  if (dist == -1) {
    target = end;
  } else {
    target = camera_p + (camera_o * glm::vec3(0, 0, -dist));
  }

  LookAt(player->GetPosition(), target, glm::vec3(0, 1, 0));

  // Fireing
  if (cooldown <= 0) {
    // Machine Gun
    if (machinegun_input && player->HasEnergy(machine_cost)) {
      glm::quat dir = GetOrientation();
      glm::vec3 bullet_v = glm::vec3(0, 0, -1);
      bullet_v *= (machinegun_bullet_speed*delta);
      EnergyBall* bullet = new EnergyBall(bullet_v, energyball_md);
      bullet->cannon = this;
      bullet->parent = player;
      bullet->ignore = {"player", "playercannon"};
      bullet->SetPosition(bullet->parent->GetPosition());
      bullet->SetOrientation(bullet->cannon->GetOrientation());
      // std::cout << "here" << std::endl;
      project->AddRigidBody(bullet);
      player->AddEnergy(-machine_cost);
      cooldown = machinegun_cooldown;
    }
  } else {
    cooldown -= delta;
  }

  RigidBody::Update(delta);
}

}  // namespace turbotanks
