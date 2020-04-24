#ifndef SRC_ENGINE_ANIMATION_H_
#define SRC_ENGINE_ANIMATION_H_

/*
 * Copyright 2020 Maui Kelley
 */

// C/C++ std lib
#include <math.h>
#include <iostream>
#include <string>
// lib
#include <glm/vec3.hpp>
#include <glm/gtx/quaternion.hpp>
// src
#include "engine/helper.h"

namespace engine {

class AnimationController;
class Animation {
 private:
  glm::vec3 position_start;
  glm::vec3 scale_start;
  glm::quat orientation_start;
  glm::vec3 position_dest;
  glm::vec3 scale_dest;
  glm::quat orientation_dest;
  float current_time;
  float length;
  int action;
  std::string transition;

 public:
  // Controller
  AnimationController* controller;

  // Default Constructor
  Animation();

  // Setters
  void SetPositionStart(const glm::vec3& pos) {position_start = pos;}
  void SetScaleStart(const glm::vec3& scale) {scale_start = scale;}
  void SetOrientationStart(const glm::quat& orient)
  {orientation_start = orient;}
  void SetPositionDestination(const glm::vec3& pos) {position_dest = pos;}
  void SetScaleDestination(const glm::vec3& scale) {scale_dest = scale;}
  void SetOrientationDestination(const glm::quat& orient)
  {orientation_dest = orient;}
  void SetLength(const float& len) {length = len;}
  void SetAction(const int& act) {action = act;}
  void SetTransition(const std::string& tran) {
    transition = tran;
    SetAction(ANIMATION_TRANSITION);
  }

  // Calculations
  glm::vec3 GetPosition();
  glm::vec3 GetScale();
  glm::quat GetOrientation();
  void NextFrame(const float& delta);
  void Reset();
};

}  // namespace engine

#endif  // SRC_ENGINE_ANIMATION_H_
