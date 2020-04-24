#ifndef SRC_ENGINE_UI_MODEL_H_
#define SRC_ENGINE_UI_MODEL_H_

/*
 * Copyright 2020 Maui Kelley
 */

// C/C++ std lib
#include <string>
#include <vector>
// lib

// src
#include "engine/model.h"

namespace engine {

class UIModel : public Model {
 private:
  float width, height, image_ratio;
  int fixed, origin;

  // Fills model data
  void CreateData();

  // fills the verticies vector
  void CreateVertices();

 public:
  float screen_ratio;
  UIModel() : Model() {
    Initialize();
  }

  // Constructor
  explicit UIModel(const std::string &image_file) : Model() {
    Initialize();
    Load(image_file);
  }

  void Initialize() {
    width = height = screen_ratio = 1.0f;
    fixed = UI_NOT_FIX;
    origin = UI_CENTER_CENTER;
    CreateData();
  }

  // Setters
  void SetWidth(float w) {
    width = w;
    CreateVertices();
  }
  void SetHeight(float h) {
    height = h;
    CreateVertices();
  }
  void Fix(int f) {
    fixed = f;
    CreateVertices();
  }
  void Align(int o) {
    origin = o;
    CreateVertices();
  }
  void SetAttributes(float w, float h, int f, int o) {
    width = w;
    height = h;
    fixed = f;
    origin = o;
    CreateVertices();
  }
  void SetScreenRatio(float ratio) {
    if (screen_ratio != ratio) {
      screen_ratio = ratio;
      CreateVertices();
    }
  }

  // Overload Load function
  void Load(const std::string &image_file);
};

}  // namespace engine

#endif  // SRC_ENGINE_UI_MODEL_H_
