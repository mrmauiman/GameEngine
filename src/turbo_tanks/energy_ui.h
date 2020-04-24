#ifndef SRC_TURBO_TANKS_ENERGY_UI_H_
#define SRC_TURBO_TANKS_ENERGY_UI_H_

/*
 * Copyright 2020 Maui Kelley
 */

#include <string>
#include "engine/ui.h"
#include "turbo_tanks/player.h"

namespace turbotanks {

class EnergyUI : public engine::UI {
 public:
  Player* player;

  explicit EnergyUI(const std::string &image_file) : engine::UI(image_file) {}

  void Update(float delta) {
    SetScale(glm::vec3(player->GetEnergyFraction(), 1, 1));
  }
};

}  // namespace turbotanks


#endif  // SRC_TURBO_TANKS_ENERGY_UI_H_
