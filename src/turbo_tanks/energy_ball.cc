/*
 * Copyright 2020 Maui Kelley
 */

#include "turbo_tanks/energy_ball.h"
#include "turbo_tanks/enemy.h"
#include "turbo_tanks/player.h"

namespace turbotanks {

// Override parent Update
void EnergyBall::Update(float delta) {
  Move(velocity);
  int o_id = project->Collides(id, ignore);
  if (life_timer >= lifespan || o_id != -1
  || !project->WithInRender(GetPosition())) {
    if (o_id != -1) {
      Enemy* en = dynamic_cast<Enemy*>(project->GetObject(o_id));
      if (en) {
        en->Hurt(10.0f);
      }
      Player* pl = dynamic_cast<Player*>(project->GetObject(o_id));
      if (pl) {
        pl->Hurt(1.0f);
      }
    }
    project->RemoveRigidBody(id);
  }
  life_timer += delta;

  RigidBody::Update(delta);
}

}  // namespace turbotanks
