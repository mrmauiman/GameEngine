/*
 * Copyright 2020 Maui Kelley
 */

#include "engine/animation.h"
#include "engine/animation_controller.h"

namespace engine {

// Default Constructor
Animation::Animation() {
  position_start = glm::vec3(0, 0, 0);
  scale_start = glm::vec3(1, 1, 1);
  orientation_start = AxisToQuat(0, glm::vec3(0, 0, 0), false);
  position_dest = glm::vec3(0, 0, 0);
  scale_dest = glm::vec3(1, 1, 1);
  orientation_dest = AxisToQuat(0, glm::vec3(0, 0, 0), false);
  current_time = 0;
  length = 1;
  action = ANIMATION_STOP;
  transition = "";
}

// Calculations
glm::vec3 Animation::GetPosition() {
  return lerp<glm::vec3>(current_time/length, position_start, position_dest);
}

glm::vec3 Animation::GetScale() {
  return lerp<glm::vec3>(current_time/length, scale_start, scale_dest);
}

glm::quat Animation::GetOrientation() {
  float start_angle = glm::angle(orientation_start);
  float dest_angle = glm::angle(orientation_dest);
  float angle = lerp<float>(current_time/length, start_angle, dest_angle);
  glm::vec3 start_axis = glm::normalize(glm::axis(orientation_start));
  glm::vec3 dest_axis = glm::normalize(glm::axis(orientation_dest));
  glm::vec3 r_axis = glm::cross(start_axis, dest_axis);
  float r_angle;
  if (glm::length(r_axis) == 0) {
    if (start_axis == dest_axis) {
      r_axis = glm::vec3(0, 0, 1);
      r_angle = 0;
    } else {
      r_axis = glm::rotate(GetAligningRotation(glm::vec3(1, 0, 0), start_axis),
      glm::vec3(0, 1, 0));
      r_angle = 180;
    }
  } else {
    r_angle = acos(glm::dot(start_axis, dest_axis));
  }
  r_axis = glm::normalize(r_axis);
  r_angle = lerp<float>(current_time/length, 0.0f, r_angle);
  glm::vec3 axis = glm::rotate(AxisToQuat(r_angle, r_axis, false), start_axis);
  return AxisToQuat(angle, axis, true);
}

void Animation::NextFrame(const float& delta) {
  current_time += delta;
  if (current_time > length) {
    switch (action) {
      case ANIMATION_STOP:
        current_time = length;
        break;
      case ANIMATION_REPEAT:
        Reset();
        break;
      case ANIMATION_TRANSITION:
        controller->Play(transition);
        break;
    }
  }
}

void Animation::Reset() {
  current_time = 0;
}

}  // namespace engine
