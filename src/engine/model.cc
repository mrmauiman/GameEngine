/*
 * Copyright 2020 Maui Kelley
 */

#include "engine/model.h"

namespace engine {

// PRIVATE

// str is a string
// returns a vector of all items in str seperated by whitespace
std::vector<std::string> Model::Tokenize(std::string str, std::string sep) {
  std::vector<std::string> tokens;
  std::string current = "";
  int c = 0;
  bool found = false;
  for (int c = 0; c < str.length(); c++) {
    found = (sep == "ws") ? (isspace(str[c])) : (str[c] == sep[0]);
    if (found) {
      if (current != "") {
        // a token was found
        tokens.push_back(current);
        current = "";
      }
    } else {
      current += str[c];
    }
    if (str[c] == '#') {
      break;
    }
  }
  if (current != "") {
    // final token
    tokens.push_back(current);
  }
  // str has been tokenized into tokens
  return tokens;
}

// vertex is a line that starts with v and contains vertex data
// Adds the vertex described in vertex
void Model::AddVertex(std::vector<std::string> vertex) {
  glm::vec4 v;
  if (vertex.size() == NUM_VERTEX_TOKENS
      || vertex.size() == NUM_VERTEX_TOKENS-1) {
    // vertex is formatted correctly
    for (int i = 1; i < vertex.size(); i++) {
      // Convert and check if vertex[i] is a valid float
      try {
        v[i-1] = std::stof(vertex[i]);
      } catch (std::exception& e) {
        throw "Invalid Vertex Definition: '" + vertex[i] + "' is not a float";
      }
    }
    if (vertex.size() == NUM_VERTEX_TOKENS-1) {
      v[VERTEX_SIZE-1] = W_DEFAULT;
    }
    verticies.push_back(v);
    // verticies contains the vertex described
  } else {
    throw "Invalid Vertex Definition: A face has " +
          std::to_string(NUM_VERTEX_TOKENS) + " or " +
          std::to_string(NUM_VERTEX_TOKENS-1) + " attributes you gave " +
          std::to_string(vertex.size());
  }
}

// face is a line that starts with f and contains face data
// Adds the face described in face and throws an exception if it's not formatted
// correctly
void Model::AddFace(std::vector<std::string> face) {
  if (face.size() == NUM_FACE_TOKENS) {
    // face is formatted correctly
    glm::vec3 f;
    for (int i = 1; i < NUM_FACE_TOKENS; i++) {
      // Convert and check if face[i] is a valid int
      try {
        std::vector<std::string> tokens = Tokenize(face[i], "/");
        int vertex_index = std::stoi(tokens[0])-1;
        int normal_index;
        if (tokens.size() == 1) {
          normal_index = -1;
        } else {
          normal_index = std::stoi((tokens.size() == 2)?tokens[1]:tokens[2]);
          normal_index--;
        }
        glm::vec2 vnp = {vertex_index, normal_index};
        // vnp is the vertex normal pair this face uses
        int index = 0;
        bool searching = true;
        while (searching && index < vertex_normal_pairs.size()) {
          if (vertex_normal_pairs[index].x == vnp.x &&
              vertex_normal_pairs[index].y == vnp.y) {
            searching = false;
          } else {
            index++;
          }
        }
        int vnp_index = index;
        if (index == vertex_normal_pairs.size()) {
          // vnp is a new vertex normal pair
          vnp_index = vertex_normal_pairs.size();
          vertex_normal_pairs.push_back(vnp);
        }
        f[i-1] = vnp_index;
      } catch(std::exception& e) {
        throw "Invalid Face Definition: '" + face[i] + "' is not an int";
      }
    }
    objects[current_material].push_back(f);
    // faces contains the face described
  } else {
    throw "Invalid Face Definition: A face has " +
          std::to_string(NUM_FACE_TOKENS) + " attributes you gave " +
          std::to_string(face.size());
  }
}

// normal is a line that starts with vn and contains normal data
// adds the normal descibed to normal_vector and returns true iff the data is
// formatted correctly and returns false otherwise
void Model::AddNormal(std::vector<std::string> normal) {
  try {
    glm::vec3 normal_v3 = {
      std::stof(normal[1]),
      std::stof(normal[2]),
      std::stof(normal[3])
    };

    normals.push_back(normal_v3);
  } catch (std::exception& e) {
    throw "Invalid Vertex Normal Definition: " + normal[0] + " " + normal[1] +
          " " + normal[2] + " " + normal[3];
  }
}

// mat_file is the name of a material library
// adds all materials described in mat_file to materials
void Model::AddMaterials(std::string mat_file) {
  std::string mat_name = "";
  std::string line;
  std::ifstream file("data/" + mat_file);
  if (file.is_open()) {
    try {
      while (getline(file, line)) {
        std::vector<std::string> tokens = Tokenize(line);
        if (tokens.size() > 0) {
          if (tokens[0] == "newmtl") {
            try {
              mat_name = tokens[1];
              materials[mat_name] = engine::Material();
            } catch (std::exception& e) {
              throw "new materials must have a name";
            }
          } else if (mat_name != "") {
          if (tokens[0] == "Ka") {
            try {
              materials.at(mat_name).SetAmbient(glm::vec3(std::stof(tokens[1]),
                             std::stof(tokens[2]), std::stof(tokens[3])));
            } catch (std::exception& e) {
              throw "Ka takes 3 arguements, " + std::to_string(tokens.size()-1)
                    + " were given.";
            }
          } else if (tokens[0] == "Kd") {
            try {
              materials.at(mat_name).SetDiffuse(glm::vec3(std::stof(tokens[1]),
                             std::stof(tokens[2]), std::stof(tokens[3])));
            } catch (std::exception& e) {
              throw "Kd takes 3 arguements, " + std::to_string(tokens.size()-1)
                    + " were given.";
            }
          } else if (tokens[0] == "Ks") {
            try {
              materials.at(mat_name).SetSpecular(glm::vec3(std::stof(tokens[1]),
                              std::stof(tokens[2]), std::stof(tokens[3])));
            } catch (std::exception& e) {
              throw "Ks takes 3 arguements, " + std::to_string(tokens.size()-1)
                    + " were given.";
            }
          } else if (tokens[0] == "Ke") {
            try {
              materials.at(mat_name).SetEmission(glm::vec3(std::stof(tokens[1]),
                              std::stof(tokens[2]), std::stof(tokens[3])));
            } catch (std::exception& e) {
              throw "Ke takes 3 arguements, " + std::to_string(tokens.size()-1)
                    + " were given.";
            }
          } else if (tokens[0] == "Ns") {
            try {
              materials.at(mat_name).SetShininess(std::stof(tokens[1]));
            } catch (std::exception& e) {
              throw "Ns takes 1 arguement, " + std::to_string(tokens.size()-1)
                    + " were given.";
            }
          }
          }
        }
      }
    } catch (const std::string msg) {
      throw "Material Library Error: " + msg;
    }
  }
}

// This is defined
// empties the verticies and faces vectors
void Model::Clear() {
  verticies.clear();
  faces.clear();
  normals.clear();
}

// returns a pointer to an array containing all verticies
// user must call delete on the value returned when they are done using it
GLfloat * Model::GetVertexData() const {
  GLfloat * rv = new GLfloat[vertex_normal_pairs.size() * VERTEX_SIZE];
  for (int i = 0; i < vertex_normal_pairs.size(); i++) {
    for (int j = 0; j < VERTEX_SIZE; j++) {
      rv[(i*VERTEX_SIZE)+j] = verticies[vertex_normal_pairs[i].x][j];
    }
  }
  return rv;
}

// returns a pointer to an array containing all normals ordered to their
// verticies
// user must call delete on the value returned when they are done using it
GLfloat * Model::GetNormalData() const {
  GLfloat * rv = new GLfloat[vertex_normal_pairs.size() * NORMAL_SIZE];
  for (int i = 0; i < vertex_normal_pairs.size(); i++) {
    for (int j = 0; j < NORMAL_SIZE; j++) {
      if (vertex_normal_pairs[i].y == -1) {
        rv[(i*NORMAL_SIZE)+j] = 0;
      } else {
        rv[(i*NORMAL_SIZE)+j] = normals[vertex_normal_pairs[i].y][j];
      }
    }
  }
  return rv;
}

// material name is the material the faces we want is associated with
// returns a pointer to an array of indicies of verticies that make up faces
// user must call delete on the value returned when they are done using it
GLuint * Model::GetFaceData(const std::string material_name) const {
  std::vector<glm::vec3> faces = objects.at(material_name);
  GLuint * rv = new GLuint[faces.size() * FACE_SIZE];
  for (int i = 0; i < faces.size(); i++) {
    for (int j = 0; j < FACE_SIZE; j++) {
      rv[(i*FACE_SIZE)+j] = faces[i][j];
    }
  }
  return rv;
}

// PUBLIC

// Default Constructor
Model::Model() {
  // Set the current material to the default material
  current_material = "engine::default";
  materials[current_material] = engine::Material();
  objects[current_material] = std::vector<glm::vec3>();
}

// obj_file_name is the path to an .obj file
// the .obj file specified is loaded into this
Model::Model(const std::string &obj_file_name) {
  // Set the current material to the default material
  current_material = "engine::default";
  materials[current_material] = engine::Material();
  // Load obj file
  Load(obj_file_name);
}

// obj_file_name is the path to an .obj file
// the .obj file specified is loaded into this
void Model::Load(const std::string &obj_file_name) {
  // Empty Previous Data
  Clear();

  std::string line;
  std::ifstream file(obj_file_name);
  if (file.is_open()) {
    try {
      while (getline(file, line)) {
        std::vector<std::string> tokens = Tokenize(line);
        // tokens is each item on the line seperated by whitespace
        if (tokens.size() > 0) {
          if (tokens[0] == "v") {
            // Line is a vertex
            AddVertex(tokens);
          } else if (tokens[0] == "f") {
            // Line is a face
            AddFace(tokens);
          } else if (tokens[0] == "vn") {
            // Line is a normal
            AddNormal(tokens);
          } else if (tokens[0] == "mtllib") {
            AddMaterials(tokens[1]);
          } else if (tokens[0] == "usemtl") {
            try {
              current_material = tokens[1];
              if (objects.find(current_material) == objects.end()) {
                objects[current_material] = std::vector<glm::vec3>();
              }
            } catch(std::exception& e) {
              throw "Invalid Material Switch Statement";
            }
          }
        }
      }
    } catch(const std::string msg) {
      Clear();
      std::cerr << msg << std::endl;
    }
    file.close();
  }
}

// An object has been loaded
// renders the obj file loaded
void Model::Draw() const {
  GLfloat * v_data = GetVertexData();
  GLfloat * n_data = GetNormalData();

  glEnableClientState(GL_VERTEX_ARRAY);
  glVertexPointer(VERTEX_SIZE, GL_FLOAT, 0, v_data);
  glEnableClientState(GL_NORMAL_ARRAY);
  glNormalPointer(GL_FLOAT, 0, n_data);



  for (auto const& mat : materials) {
    // std::cout << mat.first << std::endl;
    if (objects.find(mat.first) != objects.end()) {
      GLuint * f_data = GetFaceData(mat.first);
      mat.second.Activate();
      glDrawElements(GL_TRIANGLES, objects.at(mat.first).size()*
      FACE_SIZE, GL_UNSIGNED_INT, f_data);
      delete f_data;
    }
  }


  delete v_data;
  delete n_data;
}

// returns the number of veriticies
int Model::GetNumVerticies() const {
  return vertex_normal_pairs.size()*VERTEX_SIZE;
}

}  // namespace engine
