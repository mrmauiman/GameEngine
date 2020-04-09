#ifndef SRC_ENGINE_MATERIAL_H_
#define SRC_ENGINE_MATERIAL_H_

/*
 * Copyright 2020 Maui Kelley
 */

// C/C++ lib
#include <GLFW/glfw3.h>
#include <string>
#include <vector>

// src
#include "engine/constants.h"
#include "engine/helper.h"

// lib
#include "glm/vec3.hpp"
#include "glm/vec4.hpp"

namespace engine {

class Material {
 private:
  float ambient[AMBIENT_SIZE];
  float diffuse[DIFFUSE_SIZE];
  float specular[SPECULAR_SIZE];
  float emission[EMISSION_SIZE];
  float shininess;

  GLuint tex_name[1];
  std::vector<GLubyte> image;

 public:
  // Default Constructor
  Material();

  // Constructor
  Material(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular,
           float shininess);

  // sets this material to the current drawing material
  void Activate() const;

  // Getters
  // returns the tex_name[0]
  GLuint GetTexName() const;

  // Setters
  // ambient is an rgba color
  void SetAmbient(glm::vec3 ambient);

  // diffuse is an rgba color
  void SetDiffuse(glm::vec3 diffuse);

  // specular is an rgba color
  void SetSpecular(glm::vec3 specular);

  // emission is an rgba color
  void SetEmission(glm::vec3 emission);

  // ambient is an rgba color
  void SetAmbient(glm::vec4 ambient);

  // diffuse is an rgba color
  void SetDiffuse(glm::vec4 diffuse);

  // specular is an rgba color
  void SetSpecular(glm::vec4 specular);

  // emission is an rgba color
  void SetEmission(glm::vec4 emission);

  // shininess is the specular exponent of range 0-128
  void SetShininess(float shininess);

  // filename is a string
  // loads the ppm file into texture
  void SetTexture(std::string filename);

  // Deconstructor
  // Free texture
  ~Material();
};

}  // namespace engine

#endif  // SRC_ENGINE_MATERIAL_H_
