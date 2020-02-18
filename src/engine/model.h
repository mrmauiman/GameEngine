#ifndef SRC_ENGINE_MODEL_H_
#define SRC_ENGINE_MODEL_H_

/*
 * Copyright 2020 Maui Kelley
 */

#include <GLFW/glfw3.h>
#include <cctype>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <exception>

#include "lib/glm/glm.hpp"
#include "lib/glm/vec3.hpp"
#include "src/engine/constants.h"

namespace engine {

class Model {
 private:
  // member data

  // Verticies is a vector of floats
  std::vector<GLfloat> verticies;

  // Faces is a vector of ints
  std::vector<GLubyte> faces;

  // Normals is a vector of floats
  std::vector<GLfloat> normals;

  // private functions

  // str is a string
  // returns a vector of all items in str seperated by whitespace
  std::vector<std::string> tokenize(std::string str);

  // vertex is a line that starts with v and contains vertex data
  // Adds the vertex described in vertex and returns true if the data is
  // formatted correctly and returns false otherwise
  bool addVertex(std::vector<std::string> vertex);

  // face is a line that starts with f and contains face data
  // Adds the face described in face and returns true if the data is
  // formatted correctly and returns false otherwise
  bool addFace(std::vector<std::string> face);

  // empties the verticies and faces vectors
  void clear();

  // calculates all the vectors based on the verticies
  void calculateNormals();

 public:
  // Default Constructor
  Model();

  // obj_file_name is the path to an .obj file
  // the .obj file specified is loaded into this
  explicit Model(const std::string &obj_file_name);

  // obj_file_name is the path to an .obj file
  // the .obj file specified is loaded into this
  void load(const std::string &obj_file_name);

  // An object has been loaded
  // renders the obj file loaded
  void draw() const;

  // returns the number of veriticies
  int getNumVerticies() const;

  // delete the copy constructor
  Model(const Model& model);

  // delete the assignment operator
  Model& operator=(const Model& model);

  // << Overload
  friend std::ostream& operator<<(std::ostream& os, const Model& md);
};
}  // namespace engine

#endif  // SRC_ENGINE_MODEL_H_
