/*
 * Copyright 2020 Maui Kelley
 */

#include "turbo_tanks/player_camera.h"

namespace turbotanks {

// Override parent Update
void PlayerCamera::Update(float delta) {
  glm::vec2 aim_input = project->GetVectorInput("aim");
  bool dev_input = project->GetButtonInput("dev");
  if (dev_input) {
    dev_cam->SetPosition(player->GetPosition());
    dev_cam->SetOrientation(0, glm::vec3(0, 0, -1), false);
    dev_cam->Move(glm::vec3(0, 5, 5));
    dev_cam->LookAt(dev_cam->GetPosition(), player->GetPosition(),
    glm::vec3(0, 1, 0));
    project->ActivateCamera(dev_cam->id);
  } else if (!enabled) {
    project->ActivateCamera(id);
  }

  look_angle += aim_input.y * vertical_sensitivity;
  look_angle = engine::clamp(look_angle, max_d_angle, max_u_angle);
  // std::cout << look_angle << std::endl;
  SetOrientation(player->GetOrientation());
  SetPosition(player->GetPosition());
  Move(glm::vec3(0.5f, 1.0f, 0.0f));
  Turn(look_angle, glm::vec3(1.0, 0.0, 0.0));
  glm::vec3 start = GetPosition();
  Move(glm::vec3(0.0f, 0.0f, 3.0f));
  glm::vec3 end = GetPosition();
  glm::vec3 temp = end-start;
  std::vector<std::string> ignore = {"player", "camera"};
  float dist = project->RayCast(start, end, ignore);
  if (dist != -1) {
    Move(glm::vec3(0, 0, -(glm::length(end-start) - dist)));
  }
}

}  // namespace turbotanks
