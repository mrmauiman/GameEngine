/*
 * Copyright 2020 Maui Kelley
 */

#include "turbo_tanks/player.h"

namespace turbotanks {

// model is a pointer to a Model
// This calls the constructor of RigidBody and also does what needs to be done
// on creation
Player::Player(const engine::Model* model): engine::RigidBody(model) {
  velocity = glm::vec2(0, 0);
  tags.push_back("player");
  bounding_box_axis_aligned = true;
  energy = max_energy;
  health = max_health;
  move.SetLength(1);
  move.SetAction(ANIMATION_REPEAT);
  animation_controller.AddAnimation("move", &move);
  animation_controller.Play("move");
}

// delta is the time the last frame took to process
// This function happens every frame
void Player::Update(float delta) {
  // Inputs
  glm::vec2 move_input = project->GetVectorInput("move") * acceleration;
  move_input.y *= -1;
  glm::vec2 aim_input = project->GetVectorInput("aim");

  // Animation
  glm::vec3 dir(move_input.x, 0, move_input.y);
  glm::vec3 up(0, 1, 0);
  glm::quat dest;
  if (dir == glm::vec3(0, 0, 0)) {
    dest = engine::AxisToQuat(0, up, false);
  } else {
    dest = engine::AxisToQuat(5, glm::cross(up, dir), false);
  }
  move.SetOrientationStart(dest);
  move.SetOrientationDestination(dest);

  // Aiming
  float turn_speed = -aim_input.x*aim_sensitivity;
  Turn(turn_speed, glm::vec3(0, 1, 0));

  // Movement
  velocity += move_input;
  if (glm::length(velocity) > max_speed) {
    velocity = glm::normalize(velocity);
    velocity *= max_speed;
  }
  // Collision
  glm::vec3 move_x(velocity.x*delta, 0, 0);
  glm::vec3 move_y(0, 0, velocity.y*delta);
  Move(move_x);
  std::vector<std::string> ignore = {"player", "playercannon", "camera",
  "energyball", "floor", "collectable"};
  // std::cout << project->Collides(id, ignore) << std::endl;
  if (project->Collides(id, ignore) != -1) {
    Move(-move_x);
    velocity.x = -velocity.x/2.0f;
  }
  Move(move_y);
  if (project->Collides(id, ignore) != -1) {
    Move(-move_y);
    velocity.y = -velocity.y/2.0f;
  }

  velocity.x -= (velocity.x * friction) * delta;
  velocity.y -= (velocity.y * friction) * delta;
  if (glm::length(velocity) < min_speed) {
    velocity = glm::vec2(0, 0);
  }

  RigidBody::Update(delta);
}

}  // namespace turbotanks
