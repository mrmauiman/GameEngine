#ifndef SRC_ENGINE_HELPER_H_
#define SRC_ENGINE_HELPER_H_

/*
 * Copyright 2020 Maui Kelley
 */

#include <GLFW/glfw3.h>
#include <math.h>
#include <fstream>
#include <iostream>
#include <vector>
#include <string>

#include "engine/constants.h"

namespace engine {

// returns returns value clamped to min and max
float clamp(float value, float min, float max);

// theta is an angle in radians
// returns the angle in degrees
float rad2deg(float theta);

// theta is an angle in degrees
// returns the angle in radians
float deg2rad(float theta);

// Check if a character is an EOL character
bool end_of_line(unsigned char c);

// Calls get on file until c is a EOL character
void skip_comment(std::ifstream * file, unsigned char * c);

// finds the P6 in ppm file and returns the 6
int get_type(std::ifstream * image_file, unsigned char * c);

// reads width, height, and max from ppm and stores them in dim
void get_dimensions(std::ifstream * image_file, unsigned char * c, int * dim);

// file name is the path to the ppm file from the project folder
// returns a GLubyte 3D array of sizes width, height, and 3
// user must delete all levels of array
std::vector<GLubyte> LoadPPM(std::string file_name, float* w, float* h);

}  // namespace engine

#endif  // SRC_ENGINE_HELPER_H_
