#ifndef SRC_ENGINE_TEXTURE_H_
#define SRC_ENGINE_TEXTURE_H_

/*
 * Copyright 2020 Maui Kelley
 */

#include <GLFW/glfw3.h>
#include <string>
#include <iostream>
#include <fstream>
#include <stdint.h>

#include "engine/constants.h"

#include "glm/vec3.hpp"
#include "glm/vec2.hpp"

namespace engine {

class Texture {
 private:
  const static int n_attr = 3;  // width, height, max
  int type;
  int attr[n_attr];
  GLubyte *** image;

  // Check if a character is an EOL character
  bool EndOfLine(unsigned char c);

  // Calls get on file until c is a EOL character
  void SkipComment(std::ifstream & file, unsigned char & c);

  // finds the P6 in ppm file and returns the 6
  int GetType(std::ifstream & image_file, unsigned char & c);

  // reads width, height, and max from ppm and stores them in attr
  void GetAttributes(std::ifstream & image_file, unsigned char & c);

 public:
  // Default Contrsuctor
  Texture();

  // Copy Constructor
  Texture(const Texture& other);

  // Load filename into memory
  void Load(std::string filename);

  // Clears memory
  void Clear();

  // x, y define a pixel in image
  // returns the rgb values of the pixel as a glm::vec3
  glm::vec3 GetPixel(int x, int y);

  // Getter for image
  GLubyte *** GetImage() const;

  // returns attr[0] and attr[1]
  glm::vec2 GetDimensions() const;

  // Deconstructor
  // calls Clear()
  ~Texture();

  // Overload = operator
  Texture& operator=(const Texture& other);
};

}  // namespace engine

#endif  // SRC_ENGINE_TEXTURE_H_
