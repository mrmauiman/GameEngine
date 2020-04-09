/*
 * Copyright 2020 Maui Kelley
 */

#include "engine/model.h"

namespace engine {

// PRIVATE

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
    bound_min.x = (v.x < bound_min.x)?v.x:bound_min.x;
    bound_min.y = (v.y < bound_min.y)?v.y:bound_min.y;
    bound_min.z = (v.z < bound_min.z)?v.z:bound_min.z;
    bound_max.x = (v.x > bound_max.x)?v.x:bound_max.x;
    bound_max.y = (v.y > bound_max.y)?v.y:bound_max.y;
    bound_max.z = (v.z > bound_max.z)?v.z:bound_max.z;
    verticies.push_back(v);
    // verticies contains the vertex described
  } else {
    throw "Invalid Vertex Definition: A face has " +
          std::to_string(NUM_VERTEX_TOKENS) + " or " +
          std::to_string(NUM_VERTEX_TOKENS-1) + " attributes you gave " +
          std::to_string(vertex.size());
  }
}

// texture vertex starts with vt and is the definition of a texture vertex
// adds the specified texture vertex to the texture_vertices and throw a
// string exception if the line is defined incorrectly
void Model::AddTextureVertex(std::vector<std::string> texture_vertex) {
  // ASSERT: texture_vertex[1] is the u coordinate and ..[2] is the v coord
  if (texture_vertex.size() == NUM_TEXTURE_VERTEX_TOKENS) {
    try {
      glm::vec2 texture_vert;
      texture_vert.x = std::stof(texture_vertex[1]);  // u coord
      texture_vert.y = std::stof(texture_vertex[2]);  // v coord
      texture_vertices.push_back(texture_vert);
    } catch (std::exception& e) {
      throw "Invalid Texture Vertex Definition: Either " + texture_vertex[1] +
            " or " + texture_vertex[2] + " is not a float";
    }
  } else {
    throw "Invalid Texture Vertex Definition: A Texture Vertex has " +
          std::to_string(NUM_TEXTURE_VERTEX_TOKENS) + "attributes you gave " +
          std::to_string(texture_vertex.size());
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
        int num_slash = std::count(face[i].begin(), face[i].end(), '/');
        int vertex_index = std::stoi(tokens[0])-1;
        int normal_index;
        int texture_index;
        if (tokens.size() == 1) {
          normal_index = 0;
          texture_index = 0;
        } else if (num_slash == 1) {
          normal_index = 0;
          texture_index = stoi(tokens[1]);
        } else {  // num_slash == 2
          normal_index = std::stoi((tokens.size() == 2)?tokens[1]:tokens[2]);
          texture_index = std::stoi((tokens.size() == 2)?"0":tokens[1]);
        }
        normal_index--;
        texture_index--;
        glm::vec3 fa = {vertex_index, texture_index, normal_index};
        // fa is the face attributes this face uses
        int index = 0;
        bool searching = true;
        while (searching && index < face_attributes.size()) {
          if (face_attributes[index].x == fa.x &&  // vertex
              face_attributes[index].y == fa.y &&  // texture vertex
              face_attributes[index].z == fa.z) {  // normal
            searching = false;
          } else {
            index++;
          }
        }
        int fa_index = index;
        if (fa_index == face_attributes.size()) {
          // fa is a new face attribute set
          face_attributes.push_back(fa);
        }
        f[i-1] = fa_index;
      } catch(std::exception& e) {
        throw "Invalid Face Definition: '" + face[i] + "' is not made of ints";
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

// mat_file is the name of a material library
// adds all materials described in mat_file to materials
void Model::AddMaterials(std::string mat_file) {
  std::string mat_name = "";
  std::string line = "";
  std::ifstream file("data/" + mat_file);
  if (file.is_open()) {
    try {
      while (getline(file, line)) {
        std::vector<std::string> tokens = Tokenize(line);
        if (tokens.size() > 0) {
          if (tokens[0] == "newmtl") {
            try {
              mat_name = tokens[1];
              materials.insert({mat_name, Material()});
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
          } else if (tokens[0] == "map_Ka") {
            if (tokens.size() == 2) {
              materials.at(mat_name).SetTexture("data/" + tokens[1]);
            } else {
              throw "map_Ka takes 1 arguement, " +
                    std::to_string(tokens.size()-1) + "were given.";
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
  texture_vertices.clear();
  normals.clear();
  face_attributes.clear();
  faces.clear();
}

// returns a pointer to an array containing all verticies
// user must call delete on the value returned when they are done using it
GLfloat * Model::GetVertexData() const {
  GLfloat * rv = new GLfloat[face_attributes.size() * VERTEX_SIZE];
  for (int i = 0; i < face_attributes.size(); i++) {
    for (int j = 0; j < VERTEX_SIZE; j++) {
      rv[(i*VERTEX_SIZE)+j] = verticies[face_attributes[i].x][j];
    }
  }
  return rv;
}

// returns a pointer to an array containing all texture vertices
// user must call delete on the value returned when they are done using it
GLfloat * Model::GetTextureVertexData() const {
  GLfloat * rv = new GLfloat[face_attributes.size() * TEXTURE_VERTEX_SIZE];
  for (int i = 0; i < face_attributes.size(); i++) {
    for (int j = 0; j < TEXTURE_VERTEX_SIZE; j++) {
      if (face_attributes[i].y == -1) {
        rv[(i*TEXTURE_VERTEX_SIZE)+j] = 0;
      } else {
        rv[(i*TEXTURE_VERTEX_SIZE)+j] =
          texture_vertices[face_attributes[i].y][j];
      }
    }
  }
  return rv;
}

// returns a pointer to an array containing all normals ordered to their
// verticies
// user must call delete on the value returned when they are done using it
GLfloat * Model::GetNormalData() const {
  GLfloat * rv = new GLfloat[face_attributes.size() * NORMAL_SIZE];
  for (int i = 0; i < face_attributes.size(); i++) {
    for (int j = 0; j < NORMAL_SIZE; j++) {
      if (face_attributes[i].z == -1) {
        rv[(i*NORMAL_SIZE)+j] = 0;
      } else {
        rv[(i*NORMAL_SIZE)+j] = normals[face_attributes[i].z][j];
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
  materials.insert({current_material, Material()});
  objects.insert({current_material, std::vector<glm::vec3>()});
  bound_min = glm::vec3(0, 0, 0);
  bound_max = glm::vec3(0, 0, 0);
}

// obj_file_name is the path to an .obj file
// the .obj file specified is loaded into this
Model::Model(const std::string &obj_file_name) {
  // Set the current material to the default material
  current_material = "engine::default";
  materials[current_material] = engine::Material();
  bound_min = glm::vec3(0, 0, 0);
  bound_max = glm::vec3(0, 0, 0);
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
          } else if (tokens[0] == "vt") {
            // Line is a texture vertex
            AddTextureVertex(tokens);
          } else if (tokens[0] == "mtllib") {
            AddMaterials(tokens[1]);
          } else if (tokens[0] == "usemtl") {
            try {
              current_material = tokens[1];
              if (objects.find(current_material) == objects.end()) {
                objects.insert({current_material, std::vector<glm::vec3>()});
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
  GLfloat * t_data = GetTextureVertexData();

  glEnableClientState(GL_VERTEX_ARRAY);
  glVertexPointer(VERTEX_SIZE, GL_FLOAT, 0, v_data);
  glEnableClientState(GL_NORMAL_ARRAY);
  glNormalPointer(GL_FLOAT, 0, n_data);
  glEnableClientState(GL_TEXTURE_COORD_ARRAY);
  glTexCoordPointer(TEXTURE_VERTEX_SIZE, GL_FLOAT, 0, t_data);

  for (auto const& mat : materials) {
    // std::cout << mat.first << std::endl;
    if (objects.find(mat.first) != objects.end()) {
      GLuint * f_data = GetFaceData(mat.first);
      mat.second.Activate();
      glDrawElements(GL_TRIANGLES, objects.at(mat.first).size()*
        FACE_SIZE, GL_UNSIGNED_INT, f_data);
      delete[] f_data;
    }
  }

  delete[] v_data;
  delete[] n_data;
  delete[] t_data;
}

// returns the number of veriticies
int Model::GetNumVerticies() const {
  return face_attributes.size()*VERTEX_SIZE;
}

// Returns bound_min
glm::vec3 Model::GetBoundMin() const {
  return bound_min;
}

// Returns bound_min
glm::vec3 Model::GetBoundMax() const {
  return bound_max;
}

}  // namespace engine
