#ifndef SRC_ENGINE_HELPER_H_
#define SRC_ENGINE_HELPER_H_

/*
 * Copyright 2020 Maui Kelley
 */

#include <math.h>

#include "src/engine/constants.h"

namespace engine {

// returns returns value clamped to min and max
float clamp(float value, float min, float max);

// theta is an angle in radians
// returns the angle in degrees
float rad2deg(float theta);

// theta is an angle in degrees
// returns the angle in radians
float deg2rad(float theta);

}  // namespace engine

#endif  // SRC_ENGINE_HELPER_H_
