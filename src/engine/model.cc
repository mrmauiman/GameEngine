/*
 * Copyright 2020 Maui Kelley
 */

#include "src/engine/model.h"

namespace engine {

// PRIVATE

// str is a string
// returns a vector of all items in str seperated by whitespace
std::vector<std::string> Model::tokenize(std::string str, std::string sep) {
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
void Model::addVertex(std::vector<std::string> vertex) {
  if (vertex.size() == NUM_VERTEX_TOKENS
      || vertex.size() == NUM_VERTEX_TOKENS-1) {
    // vertex is formatted correctly
    for (int i = 1; i < vertex.size(); i++) {
      // Convert and check if vertex[i] is a valid float
      try {
        verticies.push_back(std::stof(vertex[i]));
      } catch (std::exception& e) {
        throw "Invalid Vertex Definition: '" + vertex[i] + "' is not a float";
      }
    }
    if (vertex.size() == NUM_VERTEX_TOKENS-1) {
      verticies.push_back(W_DEFAULT);
    }
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
void Model::addFace(std::vector<std::string> face,
                    std::map<int, std::vector<int>> * normal_map) {
  if (face.size() == NUM_FACE_TOKENS) {
    // face is formatted correctly
    for (int i = 1; i < NUM_FACE_TOKENS; i++) {
      // Convert and check if face[i] is a valid int
      try {
        std::vector<std::string> tokens = tokenize(face[i], "/");
        std::string normal_index = (tokens.size() == 2) ? tokens[1] : tokens[2];
        int face_index = std::stoi(tokens[0])-1;
        if (normal_map->find(face_index) == normal_map->end()) {
          normal_map->insert(std::pair<int, std::vector<int>>
                             (face_index, std::vector<int>()));
          // normal_map[face_index] = std::vector<int>();
        }
        normal_map->at(face_index).push_back(std::stoi(normal_index)-1);
        faces.push_back(face_index);
      } catch(std::exception& e) {
        throw "Invalid Face Definition: '" + face[i] + "' is not an int";
      }
    }
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
void Model::addNormal(std::vector<std::string> normal,
                      std::vector<glm::vec3> * normal_vector) {
  try {
    glm::vec3 normal_v3 = {
      std::stof(normal[1]),
      std::stof(normal[2]),
      std::stof(normal[3])
    };

    normal_vector->push_back(normal_v3);
  } catch (std::exception& e) {
    throw "Invalid Vertex Normal Definition: " + normal[0] + " " + normal[1] +
          " " + normal[2] + " " + normal[3];
  }
}

// This is defined
// empties the verticies and faces vectors
void Model::clear() {
  verticies.clear();
  faces.clear();
}

// normal_vector is all of the normals, normal_map is the normals sorted
// populates normals with correct data from normal_vector and normal_map
void Model::calculateNormals(std::vector<glm::vec3> normal_vector,
                             std::map<int, std::vector<int>> normal_map) {
  // show content:
  for (int i = 0; i < verticies.size()/VERTEX_SIZE; i++) {
    glm::vec3 sum = normal_vector[normal_map[i][0]];
    for (int j = 1; j < normal_map[i].size(); j++) {
      sum += normal_vector[normal_map[i][j]];
    }
    glm::vec3 average = glm::normalize(sum*(1.0f/normal_map[i].size()));
    normals.push_back(average.x);
    normals.push_back(average.y);
    normals.push_back(average.z);
  }
}

// PUBLIC

// Default Constructor
Model::Model() {
  // Nothing Needs to be done
}

// obj_file_name is the path to an .obj file
// the .obj file specified is loaded into this
Model::Model(const std::string &obj_file_name) {
  load(obj_file_name);
}

// obj_file_name is the path to an .obj file
// the .obj file specified is loaded into this
void Model::load(const std::string &obj_file_name) {
  // Empty Previous Data
  clear();

  // Create variables for loading normals
  std::vector<glm::vec3> normal_vector;
  std::map<int, std::vector<int>> normal_map;

  std::string line;
  std::ifstream file(obj_file_name);
  if (file.is_open()) {
    try {
      while (getline(file, line)) {
        std::vector<std::string> tokens = tokenize(line);
        // tokens is each item on the line seperated by whitespace
        if (tokens.size() > 0) {
          if (tokens[0] == "v") {
            // Line is a vertex
            addVertex(tokens);
          } else if (tokens[0] == "f") {
            // Line is a face
            addFace(tokens, &normal_map);
          } else if (tokens[0] == "vn") {
            // Line is a normal
            addNormal(tokens, &normal_vector);
          }
        }
      }
      calculateNormals(normal_vector, normal_map);
    } catch(const std::string msg) {
      clear();
      std::cerr << msg << std::endl;
    }
    file.close();
  }
}

// An object has been loaded
// renders the obj file loaded
void Model::draw() const {
  glEnableClientState(GL_VERTEX_ARRAY);
  glVertexPointer(VERTEX_SIZE, GL_FLOAT, 0, verticies.data());
  glEnableClientState(GL_NORMAL_ARRAY);
  glNormalPointer(GL_FLOAT, 0, normals.data());

  glDrawElements(GL_TRIANGLES, faces.size(), GL_UNSIGNED_BYTE, faces.data());
}

// returns the number of veriticies
int Model::getNumVerticies() const {
  return verticies.size();
}

// << Overload
std::ostream& operator<<(std::ostream& os, const Model& md) {
    os << "{" << std::endl;
    os << "\tVerticies: {" << std::endl;
    for (int i = 0; i < md.verticies.size(); i++) {
      if (i%VERTEX_SIZE == 0) {
        os << "\t\t[";
      }

      os << md.verticies[i];

      if (i%VERTEX_SIZE == VERTEX_SIZE-1) {
        os << "]," << std::endl;
      } else {
        os << ", ";
      }
    }
    os << "\t}," << std::endl;
    os << "\tFaces: {" << std::endl;
    for (int i = 0; i < md.faces.size(); i++) {
      if (i%FACE_SIZE == 0) {
        os << "\t\t[";
      }

      os << 0+md.faces[i];

      if (i%FACE_SIZE == FACE_SIZE-1) {
        os << "]," << std::endl;
      } else {
        os << ", ";
      }
    }
    os << "\t}," << std::endl;
    os << "}";
    return os;
}

}  // namespace engine
