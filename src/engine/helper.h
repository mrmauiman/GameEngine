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
#include <map>

#include "engine/constants.h"

#include "glm/vec3.hpp"
#include "glm/gtx/quaternion.hpp"
#include "glm/gtx/vector_angle.hpp"

namespace engine {

// str is a string, sep is a character to tokenize str by, "ws" is whitespace
// returns a vector of all items in str seperated by whitespace
std::vector<std::string> Tokenize(std::string str, std::string sep = "ws");

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
// returns a GLubyte vector of size width * height * 3
std::vector<GLubyte> LoadPPM(std::string file_name, float* w, float* h);

// file name is the path to the pam file from the project folder
// returns a GLubyte vector of size width * height * depth specified in file
std::vector<GLubyte> LoadPAM(std::string fname, float* w, float* h, float* d);

// prints vec3
void PrintVec3(glm::vec3 v);

// print quat
void PrintQuat(glm::quat q);

// axis is the starting axis and d_axis is the destination axis
// returns the rotation to apply to axis to get d_axis
glm::quat GetAligningRotation(glm::vec3 axis, glm::vec3 d_axis);

// angle and axis describe an angle axis and radians is whether angle is
// radians, by default this is true
// returns a quaternion that represents the axis angle
glm::quat AxisToQuat(float angle, glm::vec3 axis, bool radians);

// axis is an axis in 3d space and point is a point in 3d space
// returns the value of how far along the axis point is
float ValueOnAxis(glm::vec3 axis, glm::vec3 point);

// min_max is a array of size 2, axis is an axis in 3D space, points is an
// array of 8 points
// sets min_max[0] to the smallest value on axis in points and min_max[1] to
// the largest
void GetMinMaxOnAxis(float * min_max, glm::vec3 axis, glm::vec3 * points);

// start and end define a line segment and face is a list of indices of points
// that creates a quad
// returns the distance on line that intersects the quad defined by face or
// -1 if it doesn't intersect
float GetCollisionOnLine(glm::vec3 start, glm::vec3 end,
                         int*face, glm::vec3* points);

// point is the point we are looking for, center is the origin, and size is the
// size of the box
bool PointInBox(glm::vec3 point, glm::vec3 center, float size);

// linear interpolation between start and end by weight
template<typename T>
T lerp(const float& weight, const T& start, const T& end) {
  return start + (weight * (end - start));
}

}  // namespace engine

#endif  // SRC_ENGINE_HELPER_H_
