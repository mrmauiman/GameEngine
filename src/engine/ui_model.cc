/*
 * Copyright 2020 Maui Kelley
 */

#include "engine/ui_model.h"

namespace engine {

void UIModel::Load(const std::string &image_file) {
  // Load texture
  materials.insert({"texture", Material()});
  materials.at("texture").SetTexture(image_file);
  materials.at("texture").SetShininess(128);
  materials.at("texture").SetAmbient(glm::vec3(0.0, 0.0, 1.0));
  materials.at("texture").SetDiffuse(glm::vec3(0.8, 0.8, 1.0));
  materials.at("texture").SetSpecular(glm::vec3(1.0, 1.0, 1.0));
  materials.at("texture").SetEmission(glm::vec3(0.2, 0.2, 0.8));
  image_ratio = materials.at("texture").GetRatio();
  if (fixed != UI_NOT_FIX) {
    CreateVertices();
  }
}

// Fills model data
void UIModel::CreateData() {
  CreateVertices();
  texture_vertices.push_back(glm::vec2(0, 0));
  texture_vertices.push_back(glm::vec2(1, 0));
  texture_vertices.push_back(glm::vec2(0, 1));
  texture_vertices.push_back(glm::vec2(1, 1));
  normals.push_back(glm::vec3(0, 0, 1));
  face_attributes.push_back(glm::vec3(0, 0, 0));
  face_attributes.push_back(glm::vec3(1, 1, 0));
  face_attributes.push_back(glm::vec3(2, 2, 0));
  face_attributes.push_back(glm::vec3(3, 3, 0));
  std::vector<glm::vec3> faces = {glm::vec3(0, 1, 3), glm::vec3(0, 2, 3)};
  objects.insert({"texture", faces});
}

// fills the verticies vector
void UIModel::CreateVertices() {
  verticies.clear();
  float w, h;
  switch (fixed) {
    case UI_NOT_FIX:
      w = 2.0f*width*screen_ratio;
      h = 2.0f*height;
      break;
    case UI_FIX_WIDTH:
      w = 2.0f*width*screen_ratio;
      h = w * (1.0f/image_ratio);
      break;
    case UI_FIX_HEIGHT:
      h = 2.0f*height;
      w = h * image_ratio;
      break;
  }
  float l, r, t, b;
  switch (origin) {
    case UI_LEFT_TOP:
    case UI_CENTER_TOP:
    case UI_RIGHT_TOP:
      t = 0;
      b = -h;
      break;
    case UI_LEFT_CENTER:
    case UI_CENTER_CENTER:
    case UI_RIGHT_CENTER:
      t = h/2.0f;
      b = -h/2.0f;
      break;
    case UI_LEFT_BOTTOM:
    case UI_CENTER_BOTTOM:
    case UI_RIGHT_BOTTOM:
      t = h;
      b = 0;
      break;
  }
  switch (origin) {
    case UI_LEFT_TOP:
    case UI_LEFT_CENTER:
    case UI_LEFT_BOTTOM:
      l = 0;
      r = w;
      break;
    case UI_CENTER_TOP:
    case UI_CENTER_CENTER:
    case UI_CENTER_BOTTOM:
      l = -w/2.0f;
      r = w/2.0f;
      break;
    case UI_RIGHT_TOP:
    case UI_RIGHT_CENTER:
    case UI_RIGHT_BOTTOM:
      l = -w;
      r = 0;
      break;
  }
  verticies.push_back(glm::vec4(l, t, 0, W_DEFAULT));
  verticies.push_back(glm::vec4(r, t, 0, W_DEFAULT));
  verticies.push_back(glm::vec4(l, b, 0, W_DEFAULT));
  verticies.push_back(glm::vec4(r, b, 0, W_DEFAULT));
}

}  // namespace engine
