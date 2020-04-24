#ifndef SRC_TURBO_TANKS_COLLECTABLE_H_
#define SRC_TURBO_TANKS_COLLECTABLE_H_

/*
 * Copyright 2020 Maui Kelley
 */

// C/C++ Std Lib

// lib

// src
#include "engine/rigid_body.h"
#include "engine/model.h"
#include "engine/animation.h"
#include "turbo_tanks/player.h"

namespace turbotanks {

class Collectable : public engine::RigidBody {
 protected:
  // Member data
 public:
  Player* player;
  engine::Animation up;
  engine::Animation down;

  // Constructor
  explicit Collectable(const engine::Model* model) : engine::RigidBody(model) {
    tags.push_back("collectable");
    up.SetPositionStart(glm::vec3(0, 0, 0));
    up.SetPositionDestination(glm::vec3(0, 0.25, 0));
    glm::quat start = engine::AxisToQuat(1, glm::vec3(0, 1, 0), false);
    glm::quat end = engine::AxisToQuat(180, glm::vec3(0, 1, 0), false);
    up.SetOrientationStart(start);
    up.SetOrientationDestination(end);
    up.SetTransition("down");
    up.SetLength(1.0f);
    down.SetPositionStart(glm::vec3(0, 0.25, 0));
    down.SetPositionDestination(glm::vec3(0, 0, 0));
    start = engine::AxisToQuat(180, glm::vec3(0, -1, 0), false);
    end = engine::AxisToQuat(359, glm::vec3(0, -1, 0), false);
    down.SetOrientationStart(start);
    down.SetOrientationDestination(end);
    down.SetTransition("up");
    down.SetLength(1.0f);
    animation_controller.AddAnimation("up", &up);
    animation_controller.AddAnimation("down", &down);
    animation_controller.Play("up");
  }

  // Check for pick up
  void Update(float delta);

  // virtual function to be implemented in children
  virtual void Collect() = 0;
};

}  // namespace turbotanks

#endif  // SRC_TURBO_TANKS_COLLECTABLE_H_
