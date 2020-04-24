#ifndef SRC_TURBO_TANKS_PLAYER_CAMERA_H_
#define SRC_TURBO_TANKS_PLAYER_CAMERA_H_

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
#include "engine/camera.h"
#include "engine/project.h"
#include "engine/helper.h"
#include "turbo_tanks/player.h"

namespace turbotanks {

class Player;
class PlayerCamera : public engine::Camera {
 private:
  const float vertical_sensitivity = 5;
  const float max_u_angle = 25;
  const float max_d_angle = -18;
  float look_angle;
 public:
  Player* player;
  engine::Camera* dev_cam;

  // Constructor
  PlayerCamera(float fov, float ner, float far):engine::Camera(fov, ner, far) {
    look_angle = 0.0f;
    tags.push_back("playercamera");
  }

  // Override parent Update
  void Update(float delta);
};

}  // namespace turbotanks

#endif  // SRC_TURBO_TANKS_PLAYER_CAMERA_H_
