/*
 * Copyright 2020 Maui Kelley
 */

#include "engine/helper.h"

namespace engine {

// str is a string
// returns a vector of all items in str seperated by whitespace
std::vector<std::string> Tokenize(std::string str, std::string sep) {
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

// returns returns value clamped to min and max
float clamp(float value, float min, float max) {
  if (min > value) {
    value = min;
  } else if (max < value) {
    value = max;
  }
  return value;
}

// theta is an angle in radians
// returns the angle in degrees
float rad2deg(float theta) {
  return (theta/PI) * 180;
}

// theta is an angle in degrees
// returns the angle in radians
float deg2rad(float theta) {
  return (theta/180) * PI;
}

// Check if a character is an EOL character
bool end_of_line(unsigned char c) {
  return (c == '\0' || c == '\n');
}

// Calls get on file until c is a EOL character
void skip_comment(std::ifstream * file, unsigned char * c) {
  while (!end_of_line(*c)) {
    *c = file->get();
  }
}

// finds the P6 in ppm file and returns the 6
int get_type(std::ifstream * image_file, unsigned char * c) {
  bool found = false;
  while (!found) {
    *c = image_file->get();
    if (*c == '#') {
      skip_comment(image_file, c);
    }
    if (*c == 'P') {
      found = true;
    }
  }
  return static_cast<int>(static_cast<char>(image_file->get())-'0');
}

// reads width, height, and max from ppm and stores them in dim
void get_ppm_dimensions(std::ifstream* image_file, unsigned char* c, int* dim) {
  *c = image_file->get();
  if (*c == '#') {
    skip_comment(image_file, c);
  }
  for (int i = 0; i < NUM_PPM_ATTRIBUTES; i++) {
    while (std::isspace(*c)) {
      *c = image_file->get();
      if (*c == '#') {
        skip_comment(image_file, c);
      }
    }
    std::string c_dim = "";
    while (!std::isspace(*c)) {
      c_dim += *c;
      *c = image_file->get();
      if (*c == '#') {
        skip_comment(image_file, c);
      }
    }
    dim[i] = std::stoi(c_dim);
  }
}

// file name is the path to the ppm file from the project folder
// returns a GLubyte vector of size width * height * 3
std::vector<GLubyte> LoadPPM(std::string file_name, float* w, float* h) {
  int type;  // always 6
  int dim[NUM_PPM_ATTRIBUTES];  // 0 is width, 1 is height, and 2 is max
  std::vector<GLubyte> image;  // How the image will be stored
  unsigned char c;  // character buffer for reading file

  // open file for reading binary
  std::ifstream image_file(file_name, std::ios::in | std::ios::binary);

  // Get the type (always 6)
  type = get_type(&image_file, &c);

  // Get the dimensions
  get_ppm_dimensions(&image_file, &c, dim);

  // Find start of data block is always a single whitespace char from max
  c = image_file.get();

  // Load the image on the stack
  for (int x = 0; x < dim[0]; x++) {
    for (int y = 0; y < dim[1]; y++) {
      for (int z = 0; z < RGB_SIZE; z++) {
        image.push_back((GLubyte)(static_cast<float>(c)/dim[2]*RGB_MAX));
        c = image_file.get();
      }
      image.push_back((GLubyte)RGB_MAX);
    }
  }
  image_file.close();

  *w = dim[0];
  *h = dim[1];
  return image;
}

// file name is the path to the pam file from the project folder
// returns a GLubyte vector of size width * height * depth specified in file
std::vector<GLubyte> LoadPAM(std::string fname, float* w, float* h, float* d) {
  int type;  // always 7
  std::map<std::string, int> dim;
  std::vector<GLubyte> image;  // How the image will be stored
  // open file for reading binary
  std::ifstream image_file(fname, std::ios::in | std::ios::binary);

  // Read Header
  std::string buf = "";
  std::string lbl = "";
  std::vector<std::string> tokens;
  while (lbl != "ENDHDR") {
    if (tokens.size() > 1 && tokens[0][0] != '#' && tokens[0] != "TUPLTYPE") {
      dim.insert({tokens[0], std::stoi(tokens[1])});
    }
    std::getline(image_file, buf);
    tokens = Tokenize(buf);
    if (tokens.size() > 0) {
      lbl = tokens[0];
    }
  }

  unsigned char c;  // character buffer for reading file
  // Load the image on the stack
  for (int x = 0; x < dim["WIDTH"]; x++) {
    for (int y = 0; y < dim["HEIGHT"]; y++) {
      for (int z = 0; z < dim["DEPTH"]; z++) {
        c = image_file.get();
        image.push_back((GLubyte)(static_cast<float>(c)/dim["MAXVAL"]*RGB_MAX));
      }
    }
  }
  image_file.close();

  *w = dim["WIDTH"];
  *h = dim["HEIGHT"];
  *d = dim["DEPTH"];
  return image;
}

// prints vec3
void PrintVec3(glm::vec3 v) {
  std::cout << v.x << " " << v.y << " " << v.z << std::endl;
}

void PrintQuat(glm::quat q) {
  glm::vec3 axis = glm::normalize(glm::axis(q));
  std::cout << rad2deg(glm::angle(q)) << "d (" << axis.x << ", " << axis.y <<
  ", " << axis.z << ")" << std::endl;
}

// GLM MATHS
// s_axis is the starting axis and d_axis is the destination axis
// returns the rotation to apply to s_zxis to get d_axis
glm::quat GetAligningRotation(glm::vec3 axis, glm::vec3 d_axis) {
  axis = glm::normalize(axis);
  d_axis = glm::normalize(d_axis);
  glm::vec3 r_axis = glm::cross(axis, d_axis);
  float angle = glm::angle(axis, d_axis);
  if (glm::length(r_axis) == 0) {
    r_axis = glm::vec3(0, 0, 1);
    angle = 0;
  } else {
    r_axis = glm::normalize(r_axis);
  }
  return AxisToQuat(angle, r_axis, true);
}

// angle and axis describe an angle axis and radians is whether angle is
// radians, by default this is true
// returns a quaternion that represents the axis angle
glm::quat AxisToQuat(float angle, glm::vec3 axis, bool radians) {
  if (!radians) {
    angle = deg2rad(angle);
  }
  return glm::angleAxis(angle, axis);
}

// axis is an axis in 3d space and point is a point in 3d space
// returns the value of how far along the axis point is
float ValueOnAxis(glm::vec3 axis, glm::vec3 point) {
  point = glm::rotate(GetAligningRotation(axis, glm::vec3(1, 0, 0)), point);
  return point.x;
}

// min_max is a array of size 2, axis is an axis in 3D space, points is an
// array of 8 points
// sets min_max[0] to the smallest value on axis in points and min_max[1] to
// the largest
void GetMinMaxOnAxis(float * min_max, glm::vec3 axis,
                                 glm::vec3 * points) {
  min_max[0] = ValueOnAxis(axis, points[0]);
  min_max[1] = ValueOnAxis(axis, points[0]);
  for (int i = 1; i < NUM_BOX_POINTS; i++) {
    float val = ValueOnAxis(axis, points[i]);
    min_max[0] = (min_max[0] > val)?val:min_max[0];
    min_max[1] = (min_max[1] < val)?val:min_max[1];
  }
}

// start and end define a line segment and face is a list of indices of points
// that creates a quad
// returns the distance on line that intersects the quad defined by face or
// -1 if it doesn't intersect
float GetCollisionOnLine(glm::vec3 start, glm::vec3 end,
                                     int*face, glm::vec3* points) {
  // Get rotaion angle and axis for aligning to z axis
  float rv = -1;
  glm::quat rot = GetAligningRotation(end-start, glm::vec3(0, 0, 1));

  // align face to z axis
  glm::vec3 aligned_f[4];
  glm::vec3 rec[4];
  for (int i = 0; i < 4; i++) {
    aligned_f[i] = glm::rotate(rot, points[face[i]]-start);
    rec[i] = glm::vec3(aligned_f[i].x, aligned_f[i].y, 0);
  }

  // Get rotation angle and axis to rotate the system to align the the square
  // to the positive x y plane
  rot = GetAligningRotation(rec[1]-rec[0], glm::vec3(1, 0, 0));

  // align face to xy plane
  for (int i = 0; i < 4; i++) {
    rec[i] = glm::rotate(rot, rec[i]);
  }

  // check if collides
  bool check[4] = {false, false, false, false};
  int assoc[4];
  for (int i = 0; i < 4; i++) {
    if (rec[i].x <= 0 && rec[i].y >= 0) {
      check[0] = true;
      assoc[0] = i;
    }
    if (rec[i].x >= 0 && rec[i].y >= 0) {
      check[1] = true;
      assoc[1] = i;
    }
    if (rec[i].x <= 0 && rec[i].y <= 0) {
      check[2] = true;
      assoc[2] = i;
    }
    if (rec[i].x >= 0 && rec[i].y <= 0) {
      check[3] = true;
      assoc[3] = i;
    }
  }

  if (check[0] && check[1] && check[2] && check[3]) {
    // There is a collision
    glm::vec3 p1 = aligned_f[assoc[0]];
    glm::vec3 p2 = aligned_f[assoc[1]];
    glm::vec3 p3 = aligned_f[assoc[2]];
    glm::vec3 v1 = p2 - p1;
    glm::vec3 v2 = p3 - p1;
    glm::vec3 perp = glm::cross(v1, v2);
    // magic formula derived from a(x-x0)+b(y-y0)+c(z-z0)=0 where x=0,y=0,z=?
    // ((a*x0 + b*y0)/c)+z0 = z
    float dist = (((perp.x*p1.x) + (perp.y*p1.y))/perp.z)+p1.z;
    if (dist >= 0.01 && dist <= glm::length(end-start)) {
      // std::cout << dist << std::endl;
      rv = dist;
    }
  }

  return rv;
}

// point is the point we are looking for, center is the origin, and size is the
// size of the box
bool PointInBox(glm::vec3 point, glm::vec3 center, float size) {
  float top = center.z + size;
  float bottom = center.z - size;
  float right = center.x + size;
  float left = center.x - size;
  return (point.z <= top && point.z >= bottom
  && point.x <= right && point.x >= left);
}

}  // namespace engine
