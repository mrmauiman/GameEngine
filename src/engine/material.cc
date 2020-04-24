/*
 * Copyright 2020 Maui Kelley
 */

#include "engine/material.h"

namespace engine {

// Default Constructor
Material::Material() {
  SetAmbient(glm::vec4(0.2f, 0.2f, 0.2f, 0.0f));
  SetDiffuse(glm::vec4(0.8f, 0.8f, 0.8f, 0.0f));
  SetSpecular(glm::vec4(0.0f, 0.0f, 0.0f, 0.0f));
  SetEmission(glm::vec4(0.0f, 0.0f, 0.0f, 0.0f));
  SetShininess(0.0f);
  tex_name[0] = -1;
  image_width = image_height = 0;
}

// Constructor
Material::Material(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular,
         float shininess) {
  SetAmbient(ambient);
  SetDiffuse(diffuse);
  SetSpecular(specular);
  SetEmission(glm::vec3(0.0f, 0.0f, 0.0f));
  SetShininess(shininess);
  tex_name[0] = -1;
  image_width = image_height = 0;
}

// sets this material to the current drawing material
void Material::Activate() const {
  // Textures
  glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient);
  glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse);
  glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular);
  glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, emission);
  glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shininess);
  if (GetTexName() != -1) {
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, GetTexName());
  }
}

// returns the tex_name[0]
GLuint Material::GetTexName() const {
  return tex_name[0];
}

// returns the width/height
float Material::GetRatio() const {
  return image_width/image_height;
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

// ambient is an rgba color
void Material::SetAmbient(glm::vec4 ambient) {
  this->ambient[0] = ambient.r;
  this->ambient[1] = ambient.g;
  this->ambient[2] = ambient.b;
  this->ambient[3] = ambient.a;
}

// diffuse is an rgba color
void Material::SetDiffuse(glm::vec4 diffuse) {
  this->diffuse[0] = diffuse.r;
  this->diffuse[1] = diffuse.g;
  this->diffuse[2] = diffuse.b;
  this->diffuse[3] = diffuse.a;
}

// specular is an rgba color
void Material::SetSpecular(glm::vec4 specular) {
  this->specular[0] = specular.r;
  this->specular[1] = specular.g;
  this->specular[2] = specular.b;
  this->specular[3] = specular.a;
}

// emission is an rgba color
void Material::SetEmission(glm::vec4 emission) {
  this->emission[0] = emission.r;
  this->emission[1] = emission.g;
  this->emission[2] = emission.b;
  this->emission[3] = emission.a;
}

// shininess is the specular exponent of range 0-1000
void Material::SetShininess(float shininess) {
  this->shininess = shininess*SPECULAR_EXPONENT_MULTIPLIER;
}

// filename is a string
// loads the ppm file into texture
void Material::SetTexture(std::string filename) {
  float d;
  std::string ext = filename.substr(filename.find_last_of(".") + 1);
  if (ext == "ppm" || ext == "pam") {
    if (ext == "ppm") {
      image = LoadPPM(filename, &image_width, &image_height);
    } else {
      image = LoadPAM(filename, &image_width, &image_height, &d);
    }

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    glGenTextures(1, tex_name);
    glBindTexture(GL_TEXTURE_2D, tex_name[0]);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    // float colors[4] = {0, 0, 0, 0};
    // glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_BLEND);
    // glTexEnvfv(GL_TEXTURE_ENV, GL_TEXTURE_ENV_COLOR, colors);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image_width, image_height, 0,
      GL_RGBA, GL_UNSIGNED_BYTE, image.data());
  } else {
    std::cout << "Only PPM and PAM files are supported, you gave a " << ext <<
    " file." << std::endl;
  }
}

// Deconstructor
// Free texture
Material::~Material() {
  glDeleteTextures(1, tex_name);
}

}  // namespace engine
