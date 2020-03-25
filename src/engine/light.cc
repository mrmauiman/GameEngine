/*
 * Copyright 2020 Maui Kelley
 */

#include "engine/light.h"

namespace engine {
// PRIVATE

// r, g, and b define the color of the Light
// Creates a light with ambient light turned off, diffuse and specular Light
// set to the specified color, Constant, linear, and quadratic attenuation set
// to 1, 0, 0 respectively, Spot cutoff and exponent set to 180 and 1
// respectively, Position set to the origin, Spot direction set down the
// negative z axis, light is active, or 'on'.
void Light::SetDefaults(float r, float g, float b) {
  try {
    light = NextAvailableLight();
  } catch (const char* msg) {
    cout << msg << endl;
    exit(0);  // An error occured stop the program
  }
  GLfloat ambient[] = {0.0f, 0.0f, 0.0f, 1.0f};
  GLfloat color[] = {r, g, b, 1.0f};

  glLightfv(light, GL_AMBIENT, ambient);
  glLightfv(light, GL_DIFFUSE, color);
  glLightfv(light, GL_SPECULAR, color);
  glLightf(light, GL_CONSTANT_ATTENUATION, 1);
  glLightf(light, GL_LINEAR_ATTENUATION, 0);
  glLightf(light, GL_QUADRATIC_ATTENUATION, 0);
  glLightf(light, GL_SPOT_CUTOFF, 180);
  glLightf(light, GL_SPOT_EXPONENT, 1);

  SetPosition(glm::vec3(0.0f, 0.0f, 0.0f));
  SetOrientation(0.0f, glm::vec3(0.0f, 0.0f, -1.0f));

  glEnable(light);
}

// returns the GLenum of the next available light and throws an exception if all
// 8 are in use
GLenum Light::NextAvailableLight() {
  // If a light has a diffuse alpha of zero we will asume it isn't in use
  GLfloat diffuse[NUM_DIFFUSE_PARAMS];
  GLenum lights[] = {GL_LIGHT0, GL_LIGHT1, GL_LIGHT2, GL_LIGHT3, GL_LIGHT4,
                     GL_LIGHT5, GL_LIGHT6, GL_LIGHT7};
  for (int i = 0; i < MAX_LIGHTS; i++) {
    glGetLight(lights[i], GL_DIFFUSE, diffuse);
    if (diffuse[DIFFUSE_ALPHA] == 0) {
      return lights[i];
    }
  }
  throw "ERROR: All 8 lights are in use.";
}

// PUBLIC

// Default Constructor
// Calls SetDefaults with white
Light::Light() {
  SetDefaults(1.0f, 1.0f, 1.0f);
}

// Constructor
// r, g, and b define the color of the Light
// calls SetDefaults with the color
Light::Light(float r, float g, float b) {
  SetDefaults(r, g, b);
}

// Deconstructor
// Disables the set light
Light::~Light() {
  GLfloat blank[] = {0.0, 0.0, 0.0, 0.0};
  glLightfv(light, GL_DIFFUSE, blank);
  if (glIsEnabled(light)) {
    glDisable(light);
  }
}

// Setters
// r, g, and b define a color
// color is set to the color defined
void Light::SetColor(float r, float g, float b) {
  GLfloat color[] = {r, g, b, 1.0f};
  glLightfv(light, GL_DIFFUSE, color);
  glLightfv(light, GL_SPECULAR, color);
}

// cuttoff is a valid angle <= 90 and with exponent they define a spotlight
// sets the lights spotlight to the specified cutoff and exponent values
void Light::SetSpot(float cutoff, float exponent) {
  glLightf(light, GL_SPOT_CUTOFF, cutoff);
  glLightf(light, GL_SPOT_EXPONENT, exponent);
}

// Sets the lights attenuation to the specified factors
void Light::SetAttenuation(float constant, float linear, float quadratic) {
  glLightf(light, GL_CONSTANT_ATTENUATION, constant);
  glLightf(light, GL_LINEAR_ATTENUATION, linear);
  glLightf(light, GL_QUADRATIC_ATTENUATION, quadratic);
}

// Sets whether the light is on
void Light::SetActivation(bool activate) {
  if (activate) {
    glEnable(light);
  } else {
    glDisable(light);
  }
}

// Override GameObject SetPosition
void Light::SetPosition(glm::vec3 position) {
  GameObject::SetPosition(position);
  GLfloat pos[] = {position.x, position.y, position.z, 1.0f};
  glLightfv(light, GL_POSITION, pos);
}

// Override GameObject SetOrientation
void Light::SetOrientation(glm::quat orientation) {
  GameObject::SetOrientation(orientation);
  glm::vec3 axis = glm::axis(orientation);
  GLfloat spot_angle = {axis.x, axis.y, axis.z};
  glLightfv(light, GL_SPOT_DIRECTION, spot_angle);
}

}  // namespace engine
