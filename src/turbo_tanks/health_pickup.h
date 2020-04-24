#ifndef SRC_TURBO_TANKS_HEALTH_PICKUP_H_
#define SRC_TURBO_TANKS_HEALTH_PICKUP_H_

/*
 * Copyright 2020 Maui Kelley
 */

// C/C++ Std Lib

// lib

// src
#include "turbo_tanks/collectable.h"

namespace turbotanks {

class HealthPickup : public Collectable {
 private:
  const float amount = 50;

 public:
  explicit HealthPickup(const engine::Model* md) : turbotanks::Collectable(md) {
    tags.push_back("healthpickup");
  }

  // Add energy back to player
  void Collect() {
    player->AddHealth(amount);
  }
};

}  // namespace turbotanks

#endif  // SRC_TURBO_TANKS_HEALTH_PICKUP_H_
