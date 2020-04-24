#ifndef SRC_ENGINE_ANIMATION_CONTROLLER_H_
#define SRC_ENGINE_ANIMATION_CONTROLLER_H_

/*
 * Copyright 2020 Maui Kelley
 */

// C/C++ std lib
#include <iostream>
#include <string>
#include <map>
// lib
#include <glm/vec3.hpp>
#include <glm/gtx/quaternion.hpp>
// src
#include "engine/helper.h"
#include "engine/animation.h"

namespace engine {

class AnimationController {
 private:
  glm::vec3 position;
  glm::vec3 scale;
  glm::quat orientation;
  std::map<std::string, Animation*> animations;
  std::string current_animation;

 public:
  // Default Constructor
  // Must call AnimationController.Update(delta) every frame for animations to
  // play
  AnimationController();

  // delta is the amount of time since last frame in seconds
  // updates position, scale, and orientation to match the current animation
  void Update(float delta);

  // animation is a pointer to a declared animation
  // adds animation to animations at name
  void AddAnimation(std::string name, Animation* animation);

  // name is a key in animations
  // starts playing animation
  void Play(std::string name);

  // Getters
  glm::vec3 GetPosition() const {return position;}
  glm::vec3 GetScale() const {return scale;}
  glm::quat GetOrientation() const {return orientation;}
};

}  // namespace engine

#endif  // SRC_ENGINE_ANIMATION_CONTROLLER_H_
