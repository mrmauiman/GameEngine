#ifndef SRC_ENGINE_UI_H_
#define SRC_ENGINE_UI_H_

/*
 * Copyright 2020 Maui Kelley
 */

// C/C++ std lib
#include <string>
#include <vector>
// lib

// src
#include "engine/ui_model.h"
#include "engine/game_object.h"

namespace engine {

class UI : public GameObject {
 private:
  UIModel image;

 public:
  float screen_ratio;

  // Default Constructor
  UI();
  explicit UI(const std::string &image_file);

  // called by all constructors
  void Initialize();

  void Load(const std::string &image_file);

  void Draw() const;

  void Update(float delta) {
    // this->Draw();
  }

  // Getters
  glm::vec3 GetScreenPosition() const {
    float x = (position.x*2.0f)-1.0f;
    float y = (position.y*2.0f)-1.0f;
    return glm::vec3(x*screen_ratio, y, position.z);
  }

  // Setters
  void SetWidth(float w) {
    image.SetWidth(w);
  }
  void SetHeight(float h) {
    image.SetHeight(h);
  }
  void Fix(int f) {
    image.Fix(f);
  }
  void Align(int o) {
    image.Align(o);
  }
  void SetAttributes(float w, float h, int f, int o) {
    image.SetAttributes(w, h, f, o);
  }
  void SetScreenRatio(float ratio) {
    screen_ratio = ratio;
    image.SetScreenRatio(ratio);
  }
};

}  // namespace engine

#endif  // SRC_ENGINE_UI_H_
