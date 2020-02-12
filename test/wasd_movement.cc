/*
 * Copyright 2020 Maui Kelley
 */

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <iostream>
#include <vector>

#include "lib/glm/gtc/quaternion.hpp"
#include "lib/glm/vec3.hpp"
#include "src/model.h"
#include "src/constants.h"
#include "src/helper.h"
#include "src/camera.h"
#include "src/rigidbody.h"

int main(int argc, char **argv) {
  // Seed the random
  unsigned int rand_seed;

  // Initialize the library
  if (!glfwInit()) {
    return -1;
  }

  // Create a windowed mode window and its OpenGL context
  GLFWwindow* window = glfwCreateWindow(640, 640, "Island Engine", NULL, NULL);
  if (!window) {
    glfwTerminate();
    return -1;
  }

  // Make the window's context current
  glfwMakeContextCurrent(window);

  // Make star
  std::string obj_file_name = "data/star.obj";
  if (argc > 1) {
    obj_file_name = argv[1];
  }
  engine::Model md(obj_file_name);
  engine::RigidBody star(&md);

  obj_file_name = "data/cube.obj";
  engine::Model fl(obj_file_name);
  engine::RigidBody floor(&fl);

  md.setColor(1.0f, 0.0f, 0.0f, 1.0f);

  int plane_size = 50;
  int num_stars = 20;
  std::vector<GLfloat> positions;
  for (int i = 0; i < num_stars; i++) {
    GLfloat p1 = (rand_r(&rand_seed)%plane_size)-(plane_size/2);
    GLfloat p2 = -1*((rand_r(&rand_seed)%plane_size)+1);
    positions.push_back(p1);
    positions.push_back(p2);
  }

  std::vector<std::vector<GLfloat>> fl_positions;
  for (int i = 0; i < plane_size; i++) {
    std::vector<GLfloat> line;
    for (int j = 0; j < plane_size; j++) {
      int x = i - (plane_size/2);
      int z = -1 * j;
      line.push_back(x);
      line.push_back(z);
    }
    fl_positions.push_back(line);
  }

  float camera_x_angle = 0;
  float camera_y_angle = 0;
  GLfloat movespeed = 0.4f;
  GLfloat view_range = 180.0;

  engine::Camera camera(45, 0.1, 100);


  glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
  glEnable(GL_DEPTH_TEST);

  // Loop until the user closes the window
  while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
         !glfwWindowShouldClose(window)) {
    // Inputs
    int h_input = glfwGetKey(window, GLFW_KEY_A) -
                  glfwGetKey(window, GLFW_KEY_D);
    int v_input = glfwGetKey(window, GLFW_KEY_W) -
                  glfwGetKey(window, GLFW_KEY_S);

    camera.move({h_input * movespeed, 0.0f, v_input * movespeed});

    // Set the rendering viewport location and dimensions
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    camera.multProjectionMatrix(width, height);

    // Get Camera Rotation
    double c_pos_x = 0.0;
    double c_pos_y = 0.0;
    glfwGetCursorPos(window, &c_pos_x, &c_pos_y);


    // Calculate angles and axies
    camera_y_angle = static_cast<float>(((c_pos_x/width)*view_range) -
                                       (view_range/2));
    camera_x_angle = static_cast<float>(((c_pos_y/height)*view_range) -
                                       (view_range/2));
    glm::vec3 y_axis = {0.0f, 1.0f, 0.0f};
    glm::vec3 x_axis = {1.0f, 0.0f, 0.0f};

    // Turn the camera
    glm::vec3 zero_axis = {0.0f, 0.0f, -1.0f};
    camera.setOrientation(0.0f, zero_axis);
    camera.turn(camera_x_angle, x_axis, false);
    camera.turn(camera_y_angle, y_axis, false);

    // Clear the color buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);

    glPushMatrix();
      camera.multViewMatrix();
      // Create Matrix for star
      for (int i = 0; i < num_stars; i++) {
        glPushMatrix();
          glm::vec3 p = {positions[i*2], 0.0f, positions[(i*2)+1]};
          star.setPosition(p);
          star.draw();
        glPopMatrix();
      }
      // Create Floor
      for (int i = 0; i < plane_size; i++) {
        for (int j = 0; j < plane_size; j++) {
          glPushMatrix();

            if (i%2 == 0) {
              if (j%2 == 0) {
                fl.setColor(0.2f, 0.2f, 0.2f, 1.0f);
              } else {
                fl.setColor(0.5f, 0.5f, 0.5f, 1.0f);
              }
            } else {
              if (j%2 == 0) {
                fl.setColor(0.5f, 0.5f, 0.5f, 1.0f);
              } else {
                fl.setColor(0.2f, 0.2f, 0.2f, 1.0f);
              }
            }
            glm::vec3 p = {fl_positions[i][j*2], -1.0f,
                           fl_positions[i][(j*2)+1]};
            glm::vec3 s = {1.0f, 0.1f, 1.0f};
            floor.setPosition(p);
            floor.setScale(s);
            floor.draw();
          glPopMatrix();
        }
      }
    glPopMatrix();

    // Swap front and back buffers
    glfwSwapBuffers(window);

    // Poll for and process events
    glfwPollEvents();
  }

  // Clean up
  glfwDestroyWindow(window);
  glfwTerminate();

  return 0;
}
