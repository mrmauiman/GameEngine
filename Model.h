#ifndef INCLUDED_Model
#define INCLUDED_Model

#include <iostream>
#include <string>
#include <GLFW/glfw3.h>
using namespace std;

class Model{
private:
  //member data
public:
  //Default Constructor
  Model();

  //Constructor
  Model(string obj_file_name);

  //PRE: obj_file_name is the path to an .obj file
  //POST: the .obj file specified is loaded into this file
  void load(string obj_file_name);

  //PRE: An object has been loaded
  //POST: renders the obj file loaded
  void draw();

  //delete the copy constructor
  Model(const Model& model);

  //delete the assignment operator
  Model& operator=(const Model& model);
}

#endif
