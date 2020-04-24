#ifndef SRC_TURBO_TANKS_ENERGY_PICKUP_H_
#define SRC_TURBO_TANKS_ENERGY_PICKUP_H_

/*
 * Copyright 2020 Maui Kelley
 */

// C/C++ Std Lib

// lib

// src
#include "turbo_tanks/collectable.h"

namespace turbotanks {

class EnergyPickup : public Collectable {
 private:
  const float amount = 50;

 public:
  explicit EnergyPickup(const engine::Model* md) : turbotanks::Collectable(md) {
    tags.push_back("energypickup");
  }

  // Add energy back to player
  void Collect() {
    player->AddEnergy(amount);
  }
};

}  // namespace turbotanks

#endif  // SRC_TURBO_TANKS_ENERGY_PICKUP_H_
