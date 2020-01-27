#include "Model.h"

//PRIVATE

//PRE:  str is a string
//POST: returns a vector of all items in str seperated by whitespace
vector<string> Model::tokenize(string str){
  vector<string> tokens;
  string current = "";
  int c = 0;
  for(int c = 0; c < str.length(); c++){
    if(isspace(str[c])){
      if(current != ""){
        //A token was found
        tokens.push_back(current);
        current = "";
      }
    }else{
      current += str[c];
    }
    if(str[c] == '#'){
      break;
    }
  }
  if(current != ""){
    //Final token
    tokens.push_back(current);
  }
  //str has been tokenized into tokens
  return tokens;
}

//PRE:  vertex is a line that starts with v and contains vertex data
//POST: Adds the vertex described in vertex and returns true if the data is
//      formatted correctly and returns false otherwise
bool Model::addVertex(vector<string> vertex){
  bool rv = false;

  if(vertex.size() == NUM_VERTEX_TOKENS || vertex.size() == NUM_VERTEX_TOKENS-1){
    //vertex is formatted correctly
    for(int i = 1; i < vertex.size(); i++){
      //Convert and check if vertex[i] is a valid float
      try{
        verticies.push_back(stof(vertex[i]));
      }catch(exception& e){
        throw "Invalid Vertex Definition: '" + vertex[i] + "' is not a float";
      }
    }
    if (vertex.size() == NUM_VERTEX_TOKENS-1){
      verticies.push_back(W_DEFAULT);
    }
    //verticies contains the vertex described
  }else{
    throw "Invalid Vertex Definition: A face has " + to_string(NUM_VERTEX_TOKENS) + " or " + to_string(NUM_VERTEX_TOKENS-1) + " attributes you gave " + to_string(vertex.size());
  }

  return rv;
}

//PRE:  face is a line that starts with f and contains face data
//POST: Adds the face described in face and returns true if the data is
//      formatted correctly and returns false otherwise
bool Model::addFace(vector<string> face){
  bool rv = false;

  if(face.size() == NUM_FACE_TOKENS){
    //face is formatted correctly
    for(int i = 1; i < NUM_FACE_TOKENS; i++){
      //Convert and check if face[i] is a valid int
      try{
        faces.push_back(stoi(face[i])-1);
      }catch(exception& e){
        throw "Invalid Face Definition: '" + face[i] + "' is not an int";
      }
    }
    //faces contains the face described
  }else{
    throw "Invalid Face Definition: A face has " + to_string(NUM_FACE_TOKENS) + " attributes you gave " + to_string(face.size());
  }

  return rv;
}

//PRE:  This is defined
//POST: empties the verticies and faces vectors
void Model::clear(){
  verticies.clear();
  faces.clear();
}

//PUBLIC

//Default Constructor
Model::Model(){
  //Nothing Needs to be done
}

//Constructor
Model::Model(const string &obj_file_name){
  load(obj_file_name);
}

//PRE: obj_file_name is the path to an .obj file
//POST: the .obj file specified is loaded into this file
void Model::load(const string &obj_file_name){
  //Empty Previous Data
  clear();

  string line;
  ifstream file(obj_file_name);
  if(file.is_open()){
    try{
      while(getline(file, line)){
        vector<string> tokens = tokenize(line);
        //tokens is each item on the line seperated by whitespace
        if(tokens.size() > 0){
          if(tokens[0] == "v"){
            //Line is a vertex
            addVertex(tokens);
          }else if(tokens[0] == "f"){
            //Line is a face
            addFace(tokens);
          }
        }
      }
    }catch(const string msg){
      clear();
      cerr << msg << endl;
    }
    file.close();
  }
}

//PRE: An object has been loaded
//POST: renders the obj file loaded
void Model::draw(){

  GLfloat colors[verticies.size()*COLOR_SIZE];
  for(int i = 0; i < verticies.size()*COLOR_SIZE; i+=3){
    colors[i]   = 1.0f;
    colors[i+1] = 1.0f;
    colors[i+2] = 1.0f;
  }

  glEnableClientState(GL_VERTEX_ARRAY);
  glEnableClientState(GL_COLOR_ARRAY);

  GLfloat* verts = verticies.data();

  glVertexPointer(VERTEX_SIZE, GL_FLOAT, 0, verts);
  glColorPointer(COLOR_SIZE, GL_FLOAT,  0, colors);

  glDrawElements(GL_TRIANGLES, faces.size(), GL_UNSIGNED_BYTE, faces.data());
}

ostream& operator<<(ostream& os, const Model& md)
{
    os << "{" << endl;
    os << "\tVerticies: {" << endl;
    for(int i = 0; i < md.verticies.size(); i++){
      if(i%VERTEX_SIZE == 0){
        os << "\t\t[";
      }

      os << md.verticies[i];

      if(i%VERTEX_SIZE == VERTEX_SIZE-1){
        os << "]," << endl;
      }else{
        os << ", ";
      }
    }
    os << "\t}," << endl;
    os << "\tFaces: {" << endl;
    for(int i = 0; i < md.faces.size(); i++){
      if(i%FACE_SIZE == 0){
        os << "\t\t[";
      }

      os << 0+md.faces[i];

      if(i%FACE_SIZE == FACE_SIZE-1){
        os << "]," << endl;
      }else{
        os << ", ";
      }
    }
    os << "\t}," << endl;
    os << "}";
    return os;
}
