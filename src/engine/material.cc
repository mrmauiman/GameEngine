/*
 * Copyright 2020 Maui Kelley
 */

#include "engine/material.h"

namespace engine {

// Default Constructor
Material::Material() {
  SetAmbient(glm::vec3(0.2f, 0.2f, 0.2f));
  SetDiffuse(glm::vec3(0.8f, 0.8f, 0.8f));
  SetSpecular(glm::vec3(0.0f, 0.0f, 0.0f));
  SetEmission(glm::vec3(0.0f, 0.0f, 0.0f));
  SetShininess(0.0f);
}

// Constructor
Material::Material(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular,
         float shininess) {
  SetAmbient(ambient);
  SetDiffuse(diffuse);
  SetSpecular(specular);
  SetEmission(glm::vec3(0.0f, 0.0f, 0.0f));
  SetShininess(shininess);
}

// sets this material to the current drawing material
void Material::Activate() const {
  glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient);
  glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse);
  glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular);
  glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, emission);
  glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shininess);
}

// Setters
// ambient is an rgba color
void Material::SetAmbient(glm::vec3 ambient) {
  this->ambient[0] = ambient.r;
  this->ambient[1] = ambient.g;
  this->ambient[2] = ambient.b;
  this->ambient[3] = 1.0f;
}

// diffuse is an rgba color
void Material::SetDiffuse(glm::vec3 diffuse) {
  this->diffuse[0] = diffuse.r;
  this->diffuse[1] = diffuse.g;
  this->diffuse[2] = diffuse.b;
  this->diffuse[3] = 1.0f;
}

// specular is an rgba color
void Material::SetSpecular(glm::vec3 specular) {
  this->specular[0] = specular.r;
  this->specular[1] = specular.g;
  this->specular[2] = specular.b;
  this->specular[3] = 1.0f;
}

// emission is an rgba color
void Material::SetEmission(glm::vec3 emission) {
  this->emission[0] = emission.r;
  this->emission[1] = emission.g;
  this->emission[2] = emission.b;
  this->emission[3] = 1.0f;
}

// shininess is the specular exponent of range 0-1000
void Material::SetShininess(float shininess) {
  this->shininess = shininess*SPECULAR_EXPONENT_MULTIPLIER;
}

}  // namespace engine
