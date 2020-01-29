#ifndef INCLUDED_Model
#define INCLUDED_Model

#include <iostream>
#include <string>
#include <vector>
#include <cctype>
#include <fstream>
#include <exception>
#include <GLFW/glfw3.h>

#include "constants.h"

namespace engine {

class Model{
private:
  //member data

  //Verticies is a vector of float vectors
  std::vector<GLfloat> verticies;

  //Faces is a vector of int vectors
  std::vector<GLubyte> faces;

  //Color is the color to draw the model
  std::vector<GLfloat> color = {1.0f, 1.0f, 1.0f, 1.0f};

  //private functions

  //PRE:  str is a string
  //POST: returns a vector of all items in str seperated by whitespace
  std::vector<std::string> tokenize(std::string str);

  //PRE:  vertex is a line that starts with v and contains vertex data
  //POST: Adds the vertex described in vertex and returns true if the data is
  //      formatted correctly and returns false otherwise
  bool addVertex(std::vector<std::string> vertex);

  //PRE:  face is a line that starts with f and contains face data
  //POST: Adds the face described in face and returns true if the data is
  //      formatted correctly and returns false otherwise
  bool addFace(std::vector<std::string> face);

  //PRE:  This is defined
  //POST: empties the verticies and faces vectors
  void clear();

public:
  //Default Constructor
  Model();

  //Constructor
  Model(const std::string &obj_file_name);

  //PRE: obj_file_name is the path to an .obj file
  //POST: the .obj file specified is loaded into this file
  void load(const std::string &obj_file_name);

  //PRE: An object has been loaded
  //POST: renders the obj file loaded
  void draw();

  //PRE:  r is the red value, g is the green value, b is the blue value, a is the
  //      alpha value
  //POST: sets the color member data
  void setColor(GLfloat r, GLfloat g, GLfloat b, GLfloat a);

  //delete the copy constructor
  Model(const Model& model);

  //delete the assignment operator
  Model& operator=(const Model& model);

  //<< Overload
  friend std::ostream& operator<<(std::ostream& os, const Model& md);
};

}

#endif
