/*
 * Copyright 2020 Maui Kelley
 */

#include "src/helper.h"

namespace engine {

// returns returns value clamped to min and max
float clamp(float value, float min, float max) {
  if (min > value) {
    value = min;
  } else if (max < value) {
    value = max;
  }
  return value;
}

// theta is an angle in radians
// returns the angle in degrees
float rad2deg(float theta) {
  return (theta/PI) * 180;
}

// theta is an angle in degrees
// returns the angle in radians
float deg2rad(float theta) {
  return (theta/180) * PI;
}

}  // namespace engine
