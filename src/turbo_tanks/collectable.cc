/*
 * Copyright 2020 Maui Kelley
 */

#include "turbo_tanks/collectable.h"

namespace turbotanks {

void Collectable::Update(float delta) {
  if (Intersects(*player)) {
    Collect();
    project->RemoveRigidBody(id);
  }

  RigidBody::Update(delta);
}

}  // namespace turbotanks
