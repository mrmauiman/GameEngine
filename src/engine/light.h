#ifndef SRC_ENGINE_LIGHT_H_
#define SRC_ENGINE_LIGHT_H_

/*
 * Copyright 2020 Maui Kelley
 */

#include <string>
#include "engine/game_object.h"
#include "engine/constants.h"
#include "glm/gtx/quaternion.hpp"
#include "glm/vec3.hpp"

namespace engine {

class Light : public GameObject {
 private:
  int light;

  // This will keep track of available lights
  static GLenum lights[];
  static bool used_lights[];

  // r, g, and b define the color of the Light
  // Creates a light with ambient light turned off, diffuse and specular Light
  // set to the specified color, Constant, linear, and quadratic attenuation set
  // to 1, 0, 0 respectively, Spot cutoff and exponent set to 180 and 1
  // respectively, Position set to the origin, Spot direction set down the
  // negative z axis, light is active, or 'on'.
  void SetDefaults(float r, float g, float b);

  // returns the index of the next available light and -1 if all 8 are in use
  int NextAvailableLight();

 public:
  // Default Constructor
  Light();

  // Constructor
  // r, g, and b define the color of the Light
  // Creates a light with ambient light turned off, diffuse and specular Light
  // set to the specified color, Constant, linear, and quadratic attenuation set
  // to 1, 0, 0 respectively, Spot cutoff and exponent set to 180 and 1
  // respectively, Position set to the origin, Spot direction set down the
  // negative z axis, light is active, or 'on'.
  Light(float r, float g, float b);

  // Deconstructor
  // Disables the set light
  ~Light();

  // Setters
  // r, g, and b define a color
  // color is set to the color defined
  void SetColor(float r, float g, float b);

  // cuttoff is a valid angle and with exponent they define a spotlight
  // sets the lights spotlight to the specified cutoff and exponent values
  void SetSpot(float cutoff, float exponent);

  // Sets the lights attenuation to the specified factors
  void SetAttenuation(float constant, float linear, float quadratic);

  // Sets whether the light is on
  void SetActivation(bool activate);

  // Override GameObject SetPosition
  void SetPosition(glm::vec3 position);

  // x, y, and z describe a location
  // sets this.position to x, y, z
  void SetPosition(float x, float y, float z);

  // Override GameObject SetOrientation
  void SetOrientation(glm::quat orientation);

  // delta is the fraction of a second a frame takes
  void Update(float delta);
};

}  // namespace engine

#endif  // SRC_ENGINE_LIGHT_H_
