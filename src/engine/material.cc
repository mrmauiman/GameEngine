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

// Copy Constructor
Material::Material(const Material& other) {
  SetAmbient(glm::vec3(0.2f, 0.2f, 0.2f));
  SetDiffuse(glm::vec3(0.8f, 0.8f, 0.8f));
  SetSpecular(glm::vec3(0.0f, 0.0f, 0.0f));
  SetEmission(glm::vec3(0.0f, 0.0f, 0.0f));
  SetShininess(0.0f);
  *this = other;
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

// returns texture.image
GLubyte *** Material::GetTexture() const {
  return texture.GetImage();
}

// returns the width and height of texture
glm::vec2 Material::GetTextureDimensions() const {
  return texture.GetDimensions();
}

// returns the tex_name[0]
GLuint Material::GetTexName() const {
  return tex_name[0];
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

// filename is a string
// loads the ppm file into texture
void Material::SetTexture(std::string filename) {
  texture.Clear();
  texture.Load(filename);
  glGenTextures(1, tex_name);

  glBindTexture(GL_TEXTURE_2D, tex_name[0]);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
                 GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                 GL_NEAREST);
  glm::vec2 dim = GetTextureDimensions();
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, dim.x,
              dim.y, 0, GL_RGB, GL_UNSIGNED_BYTE,
              GetTexture());
}

//overload =
Material& Material::operator=(const Material& other) {
  for (int i = 0; i < AMBIENT_SIZE; i++) {
    ambient[i] = other.ambient[i];
  }
  for (int i = 0; i < AMBIENT_SIZE; i++) {
    diffuse[i] = other.diffuse[i];
  }
  for (int i = 0; i < AMBIENT_SIZE; i++) {
    specular[i] = other.specular[i];
  }
  for (int i = 0; i < AMBIENT_SIZE; i++) {
    emission[i] = other.emission[i];
  }
  shininess = other.shininess;
  texture = other.texture;
  tex_name[0] = other.tex_name[0];
  return *this;
}

}  // namespace engine
