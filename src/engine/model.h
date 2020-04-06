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
#include <exception>
#include <algorithm>

#include "glm/glm.hpp"
#include "glm/vec2.hpp"
#include "glm/vec3.hpp"
#include "glm/vec4.hpp"
#include "engine/material.h"
#include "engine/constants.h"

namespace engine {

class Model {
 private:
  // member data

  // Verticies is a c++ vector of vectors
  std::vector<glm::vec4> verticies;

  // texture_vertices is a c++ vector of vectors
  std::vector<glm::vec2> texture_vertices;

  // Normals is a c++ vector of vectors
  std::vector<glm::vec3> normals;

  // face attributes stores the connections between vertices, texture vertices,
  // and normals
  std::vector<glm::vec3> face_attributes;

  // Faces is a c++ vector of vectors
  std::vector<glm::vec3> faces;

  // objects is a map that matches material names to groups of faces
  std::map<std::string, std::vector<glm::vec3>> objects;

  // materials is a map that matches material names to materials
  std::map<std::string, engine::Material> materials;

  // current_material is the material to assign to any face read in
  std::string current_material;



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

  // texture vertex starts with vt and is the definition of a texture vertex
  // adds the specified texture vertex to the texture_vertices and throw a
  // string exception if the line is defined incorrectly
  void AddTextureVertex(std::vector<std::string> texture_vertex);

  // normal is a line that starts with vn and contains normal data
  // adds the normal descibed to normal_vector and returns true iff the data is
  // formatted correctly and returns false otherwise
  void AddNormal(std::vector<std::string> normal);

  // mat_file is the name of a material library
  // adds all materials described in mat_file to materials
  void AddMaterials(std::string mat_file);

  // empties the verticies and faces vectors
  void Clear();

  // returns a pointer to an array containing all verticies
  // user must call delete on the value returned when they are done using it
  GLfloat * GetVertexData() const;

  // returns a pointer to an array containing all texture vertices
  // user must call delete on the value returned when they are done using it
  GLfloat * GetTextureVertexData() const;

  // returns a pointer to an array containing all normals ordered to their
  // verticies
  // user must call delete on the value returned when they are done using it
  GLfloat * GetNormalData() const;

  // material name is the material the faces we want is associated with
  // returns a pointer to an array of indicies of verticies that make up faces
  // user must call delete on the value returned when they are done using it
  GLuint * GetFaceData(const std::string material_name) const;

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
