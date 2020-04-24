/*
 * Copyright 2020 Maui Kelley
 */

#include "engine/animation_controller.h"
#include "engine/animation.h"

namespace engine {

// Default Constructor
AnimationController::AnimationController() {
  position = glm::vec3(0, 0, 0);
  scale = glm::vec3(1, 1, 1);
  orientation = AxisToQuat(0, glm::vec3(0, 0, 0), false);
  current_animation = "";
}

// delta is the amount of time since last frame in seconds
// updates position, scale, and orientation to match the current animation
void AnimationController::Update(float delta) {
  if (animations.find(current_animation) != animations.end()) {
    Animation* anim = animations[current_animation];
    position = anim->GetPosition();
    scale = anim->GetScale();
    orientation = anim->GetOrientation();
    anim->NextFrame(delta);
  }
}

// animation is a pointer to a declared animation
// adds animation to animations at name
void AnimationController::AddAnimation(std::string name, Animation* animation) {
  animations[name] = animation;
  animation->controller = this;
}

// name is a key in animations
// starts playing animation
void AnimationController::Play(std::string name) {
  current_animation = name;
  animations[current_animation]->Reset();
}

}  // namespace engine
