/*
 * Copyright 2020 Maui Kelley
 */

#include "engine/light.h"

namespace engine {
// PRIVATE

// Initialize static member data
GLenum Light::lights[] = {GL_LIGHT0, GL_LIGHT1, GL_LIGHT2, GL_LIGHT3,
                          GL_LIGHT4, GL_LIGHT5, GL_LIGHT6, GL_LIGHT7};
bool Light::used_lights[] = {false, false, false, false,
                             false, false, false, false};

// r, g, and b define the color of the Light
// Creates a light with ambient light turned off, diffuse and specular Light
// set to the specified color, Constant, linear, and quadratic attenuation set
// to 1, 0, 0 respectively, Spot cutoff and exponent set to 180 and 1
// respectively, Position set to the origin, Spot direction set down the
// negative z axis, light is active, or 'on'.
void Light::SetDefaults(float r, float g, float b) {
  light = NextAvailableLight();
  GLfloat ambient[] = {0.0f, 0.0f, 0.0f, 1.0f};
  SetPosition(glm::vec3(0.0f, 0.0f, 0.0f));
  GameObject::SetOrientation(0.0f, glm::vec3(0.0f, 0.0f, -1.0f), false);

  if (light != -1) {
    glLightfv(lights[light], GL_AMBIENT, ambient);
    SetColor(r, g, b);
    SetAttenuation(1, 0, 0);
    SetSpot(180, 1);
    SetActivation(true);
  }
}

// returns the index of the next available light and -1 if all 8 are in use
int Light::NextAvailableLight() {
  for (int i = 0; i < MAX_LIGHTS; i++) {
    if (!used_lights[i]) {
      used_lights[i] = true;
      return i;
    }
  }
  return -1;
}

// PUBLIC

// Default Constructor
// Calls SetDefaults with white
Light::Light() {
  SetDefaults(1.0f, 1.0f, 1.0f);
  glEnable(GL_LIGHTING);
}

// Constructor
// r, g, and b define the color of the Light
// calls SetDefaults with the color
Light::Light(float r, float g, float b) {
  SetDefaults(r, g, b);
  glEnable(GL_LIGHTING);
}

// Deconstructor
// Disables the set light and frees the light for use
Light::~Light() {
  if (light != -1) {
    used_lights[light] = false;
    if (glIsEnabled(lights[light])) {
      glDisable(lights[light]);
    }
  }
}

// Setters
// r, g, and b define a color
// color is set to the color defined
void Light::SetColor(float r, float g, float b) {
  if (light != -1) {
    GLfloat color[] = {r, g, b, 1.0f};
    glLightfv(lights[light], GL_DIFFUSE, color);
    glLightfv(lights[light], GL_SPECULAR, color);
  }
}

// cuttoff is a valid angle <= 90 and with exponent they define a spotlight
// sets the lights spotlight to the specified cutoff and exponent values
void Light::SetSpot(float cutoff, float exponent) {
  if (light != -1) {
    glLightf(lights[light], GL_SPOT_CUTOFF, cutoff);
    glLightf(lights[light], GL_SPOT_EXPONENT, exponent);
  }
}

// Sets the lights attenuation to the specified factors
void Light::SetAttenuation(float constant, float linear, float quadratic) {
  if (light != -1) {
    glLightf(lights[light], GL_CONSTANT_ATTENUATION, constant);
    glLightf(lights[light], GL_LINEAR_ATTENUATION, linear);
    glLightf(lights[light], GL_QUADRATIC_ATTENUATION, quadratic);
  }
}

// Sets whether the light is on
void Light::SetActivation(bool activate) {
  if (light != -1) {
    if (activate) {
      glEnable(lights[light]);
    } else {
      glDisable(lights[light]);
    }
  }
}

// Override GameObject SetPosition
void Light::SetPosition(glm::vec3 position) {
  GameObject::SetPosition(position);
  if (light != -1) {
    GLfloat pos[] = {position.x, position.y, position.z, 1.0f};
    glLightfv(lights[light], GL_POSITION, pos);
  }
}

// x, y, and z describe a location
// sets this.position to x, y, z
void Light::SetPosition(float x, float y, float z) {
  GameObject::SetPosition(x, y, z);
}

// Override GameObject SetOrientation
void Light::SetOrientation(glm::quat orientation) {
  GameObject::SetOrientation(orientation);
  if (light != -1) {
    glm::vec3 axis = glm::axis(orientation);
    GLfloat spot_angle[] = {axis.x, axis.y, axis.z};
    glLightfv(lights[light], GL_SPOT_DIRECTION, spot_angle);
  }
}

// angle and axis describe an angle axis and radians is whether angle is
// radians, by default this is true
// sets this.orientation to be a quaternion representing the angle axis
void Light::SetOrientation(float angle, glm::vec3 axis, bool radians) {
  GameObject::SetOrientation(angle, axis, radians);
}

// delta is the fraction of a second a frame takes
void Light::Update(float delta) {
  // Nothing to do for now
}

}  // namespace engine
