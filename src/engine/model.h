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

#include "glm/glm.hpp"
#include "glm/vec2.hpp"
#include "glm/vec3.hpp"
#include "glm/vec4.hpp"
#include "engine/constants.h"

namespace engine {

class Model {
 private:
  // member data

  // Verticies is a c++ vector of vectors
  std::vector<glm::vec4> verticies;

  // Normals is a c++ vector of vectors
  std::vector<glm::vec3> normals;

  // vertex_normal_pairs is a c++ vector of vectors
  std::vector<glm::vec2> vertex_normal_pairs;

  // Faces is a c++ vector of vectors
  std::vector<glm::vec3> faces;


  // private functions

  // str is a string, sep is a character to tokenize str by, "ws" is whitespace
  // returns a vector of all items in str seperated by whitespace
  std::vector<std::string> Tokenize(std::string str, std::string sep = "ws");

  // vertex is a line that starts with v and contains vertex data
  // Adds the vertex described in vertex and returns true if the data is
  // formatted correctly and returns false otherwise
  void AddVertex(std::vector<std::string> vertex);

  // face is a line that starts with f and contains face data
  // Adds the face described in face and returns true if the data is
  // formatted correctly and returns false otherwise
  void AddFace(std::vector<std::string> face);

  // normal is a line that starts with vn and contains normal data
  // adds the normal descibed to normal_vector and returns true iff the data is
  // formatted correctly and returns false otherwise
  void AddNormal(std::vector<std::string> normal);

  // empties the verticies and faces vectors
  void Clear();

  // returns a pointer to an array containing all verticies
  // user must call delete on the value returned when they are done using it
  GLfloat * GetVertexData() const;

  // returns a pointer to an array containing all normals ordered to their
  // verticies
  // user must call delete on the value returned when they are done using it
  GLfloat * GetNormalData() const;

  // returns a pointer to an array of indicies of verticies that make up faces
  // user must call delete on the value returned when they are done using it
  GLuint * GetFaceData() const;

 public:
  // Default Constructor
  Model();

  // obj_file_name is the path to an .obj file
  // the .obj file specified is loaded into this
  explicit Model(const std::string &obj_file_name);

  // obj_file_name is the path to an .obj file
  // the .obj file specified is loaded into this
  void Load(const std::string &obj_file_name);

  // An object has been loaded
  // renders the obj file loaded
  void Draw() const;

  // returns the number of veriticies
  int GetNumVerticies() const;

  // delete the copy constructor
  Model(const Model& model);

  // delete the assignment operator
  Model& operator=(const Model& model);
};
}  // namespace engine

#endif  // SRC_ENGINE_MODEL_H_
