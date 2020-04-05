/*
 * Copyright 2020 Maui Kelley
 */

#include "engine/texture.h"

namespace engine {
  // PRIVATE

  // Check if a character is an EOL character
  bool Texture::EndOfLine(unsigned char c) {
  	return (c == '\0' || c == '\n');
  }

  // Calls get on file until c is a EOL character
  void Texture::SkipComment(std::ifstream & file, unsigned char & c) {
  	while(!EndOfLine(c)) {
  		c = file.get();
  	}
  }

  // finds the P6 in ppm file and returns the 6
  int Texture::GetType(std::ifstream & image_file, unsigned char & c) {
  	bool found = false;
  	while (!found) {
  		c = image_file.get();
  		if (c == '#') {
  			SkipComment(image_file, c);
  		}
  		if (c == 'P') {
  			found = true;
  		}
  	}
  	return (int)(((char)image_file.get())-'0');
  }

  // reads width, height, and max from ppm and stores them in attr
  void Texture::GetAttributes(std::ifstream & image_file, unsigned char & c) {
  	c = image_file.get();
  	if (c == '#') {
  		SkipComment(image_file, c);
  	}
  	for (int i = 0; i < n_attr; i++) {
  		while (std::isspace(c)) {
  			c = image_file.get();
  			if (c == '#') {
  				SkipComment(image_file, c);
  			}
  		}
  		std::string c_attr = "";
  		while (!std::isspace(c)) {
  			c_attr += c;
  			c = image_file.get();
  			if (c == '#') {
  				SkipComment(image_file, c);
  			}
  		}
  		attr[i] = std::stoi(c_attr);
  	}
  }

  // PUBLIC

  // Default Contrsuctor
  Texture::Texture() {
    attr[0] = 0;
    attr[1] = 0;
    attr[2] = 0;
    glEnable(GL_TEXTURE_2D);
  }

  // Copy Constructor
  Texture::Texture(const Texture& other) {
    attr[0] = 0;
    attr[1] = 0;
    attr[2] = 0;
    glEnable(GL_TEXTURE_2D);
    *this = other;
  }

  // Load filename into memory
  void Texture::Load(std::string filename) {
    unsigned char c;  // character buffer

    // open file for reading binary
  	std::ifstream image_file(filename, std::ios::in | std::ios::binary);

  	// Get the type (always 6)
  	type = GetType(image_file, c);

  	// Get the dimensions
  	GetAttributes(image_file, c);

  	// Find start of data block
  	while (std::isspace(c)) {
  		c = image_file.get();
  		if (c == '#') {
  			SkipComment(image_file, c);
  		}
  	}

  	// Load the image on the stack
  	image = new GLubyte**[attr[0]];
  	for (int x = 0; x < attr[0]; x++) {
  		image[x] = new GLubyte*[attr[1]];
  		for (int y = 0; y < attr[1]; y++) {
  			image[x][y] = new GLubyte[3];
  			image[x][y][0] = ((float)c/attr[2]) * COLOR_MAX;
  			image[x][y][1] = ((float)image_file.get()/attr[2]) * COLOR_MAX;
  			image[x][y][2] = ((float)image_file.get()/attr[2]) * COLOR_MAX;
  			c = image_file.get();
  		}
  	}
  	image_file.close();
  }

  // Clears memory
  void Texture::Clear() {
    // Clean up stack
  	for (int x = 0; x < attr[0]; x++) {
  		for (int y = 0; y < attr[1]; y++) {
  			delete image[x][y];
  		}
  		delete image[x];
  	}
  	delete image;
  }

  // x, y define a pixel in image
  // returns the rgb values of the pixel as a glm::vec3
  glm::vec3 Texture::GetPixel(int x, int y) {
    try {
      unsigned char * pixel = image[x][y];
      return glm::vec3((float)pixel[0], (float)pixel[1], (float)pixel[2]);
    } catch (std::exception& e) {
      std::cout << "Invalid pixel index (" + std::to_string(x) + ", " +
                   std::to_string(y) + ")" << std::endl;
    }
  }

  // Getter for image
  GLubyte *** Texture::GetImage() const {
    return image;
  }

  // returns attr[0] and attr[1]
  glm::vec2 Texture::GetDimensions() const {
    return glm::vec2((float)attr[0], (float)attr[1]);
  }

  // Deconstructor
  // calls Clear()
  Texture::~Texture() {
    Clear();
  }

  // Overload = operator
  Texture& Texture::operator=(const Texture& other) {
    type = other.type;
    for (int i = 0; i < n_attr; i++) {
      attr[i] = other.attr[i];
    }
    for (int x = 0; x < attr[0]; x++) {
      for (int y = 0; y < attr[1]; y++) {
        for (int c = 0; c < 3; c++) {
          image[x][y][c] = other.image[x][y][c];
        }
      }
    }
    return *this;
  }

}  // namespace engine
