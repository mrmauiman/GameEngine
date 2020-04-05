#ifndef SRC_ENGINE_MATERIAL_H_
#define SRC_ENGINE_MATERIAL_H_

/*
 * Copyright 2020 Maui Kelley
 */

// C/C++ lib
#include <GLFW/glfw3.h>

// src
#include "engine/constants.h"
#include "engine/texture.h"

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
  Texture texture;

  GLuint tex_name[1];

 public:
  // Default Constructor
  Material();

  // Copy Constructor
  Material(const Material& other);

  // Constructor
  Material(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular,
           float shininess);

  // sets this material to the current drawing material
  void Activate() const;

  // Getters
  // returns texture.image
  GLubyte *** GetTexture() const;

  // returns the width and height of texture
  glm::vec2 GetTextureDimensions() const;

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

  // shininess is the specular exponent of range 0-128
  void SetShininess(float shininess);

  // filename is a string
  // loads the ppm file into texture
  void SetTexture(std::string filename);

  //overload =
  Material& operator=(const Material& other);
};

}  // namespace engine

#endif  // SRC_ENGINE_MATERIAL_H_
