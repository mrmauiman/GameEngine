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

}  // namespace engine
