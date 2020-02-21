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
#include <map>
#include <utility>
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

  // str is a string, sep is a character to tokenize str by, "ws" is whitespace
  // returns a vector of all items in str seperated by whitespace
  std::vector<std::string> tokenize(std::string str, std::string sep = "ws");

  // vertex is a line that starts with v and contains vertex data
  // Adds the vertex described in vertex and returns true if the data is
  // formatted correctly and returns false otherwise
  void addVertex(std::vector<std::string> vertex);

  // face is a line that starts with f and contains face data
  // Adds the face described in face and returns true if the data is
  // formatted correctly and returns false otherwise
  void addFace(std::vector<std::string> face,
               std::map<int, std::vector<int>> * normal_map);

  // normal is a line that starts with vn and contains normal data
  // adds the normal descibed to normal_vector and returns true iff the data is
  // formatted correctly and returns false otherwise
  void addNormal(std::vector<std::string> normal,
                 std::vector<glm::vec3> * normal_vector);

  // empties the verticies and faces vectors
  void clear();

  // normal_vector is all of the normals, normal_map is the normals sorted
  // populates normals with correct data from normal_vector and normal_map
  void calculateNormals(std::vector<glm::vec3> normal_vector,
                        std::map<int, std::vector<int>> normal_map);

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

  // Calls draw()
  void Draw() const;

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
