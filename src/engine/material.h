#ifndef SRC_ENGINE_MATERIAL_H_
#define SRC_ENGINE_MATERIAL_H_

/*
 * Copyright 2020 Maui Kelley
 */

// C/C++ lib
#include <GLFW/glfw3.h>

// src
#include "engine/constants.h"

// lib
#include "glm/vec3.hpp"

namespace engine {

class Material {
 private:
  float ambient[AMBIENT_SIZE];
  float diffuse[DIFFUSE_SIZE];
  float specular[SPECULAR_SIZE];
  float emission[EMISSION_SIZE];
  float shininess;

 public:
  // Default Constructor
  Material();

  // Constructor
  Material(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular,
           float shininess);

  // sets this material to the current drawing material
  void Activate() const;

  // Setters
  // ambient is an rgba color
  void SetAmbient(glm::vec3 ambient);

  // diffuse is an rgba color
  void SetDiffuse(glm::vec3 diffuse);

  // specular is an rgba color
  void SetSpecular(glm::vec3 specular);

  // emission is an rgba color
  void SetEmission(glm::vec3 emission);

  // shininess is the specular exponent of range 0-128
  void SetShininess(float shininess);

};

}  // namespace engine

#endif  // SRC_ENGINE_MATERIAL_H_
