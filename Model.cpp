#include "Model.h"

//Default Constructor
Model(){

}

//Constructor
Model(string obj_file_name){

}

//PRE: obj_file_name is the path to an .obj file
//POST: the .obj file specified is loaded into this file
void load(string obj_file_name){
  string line;
  ifstream file(obj_file_name);
  if(file.is_open()){
    while(getline(file, line)){
      
    }
    file.close();
  }
}

//PRE: An object has been loaded
//POST: renders the obj file loaded
void draw(){

}
