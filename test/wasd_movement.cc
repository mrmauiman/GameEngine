/*
 * Copyright 2020 Maui Kelley
 */

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <iostream>
#include <vector>
#include <random>

#include "glm/gtc/quaternion.hpp"
#include "glm/vec3.hpp"
#include "engine/model.h"
#include "engine/constants.h"
#include "engine/helper.h"
#include "engine/camera.h"
#include "engine/rigid_body.h"

int main(int argc, char **argv) {
  // Seed the random
  std::random_device r;
  std::default_random_engine el(r());

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

  // Load Star Model
  std::string obj_file_name = "data/star.obj";
  if (argc > 1) {
    obj_file_name = argv[1];
  }
  engine::Model star_md(obj_file_name);

  // Load Floor Model
  obj_file_name = "data/cube.obj";
  engine::Model floor_md(obj_file_name);
  engine::RigidBody floor(&floor_md);

  // Initialize necessary variables
  const int plane_size = 50;
  const int num_stars = 20;
  const GLfloat movespeed = 0.4f;
  const GLfloat view_range = 180.0;
  float camera_x_angle = 0;
  float camera_y_angle = 0;
  bool cursor_captured = true;
  std::vector<engine::RigidBody*> stars;
  std::vector<engine::RigidBody*> floors;
  engine::Camera camera(90, 0.1, 100, false);

  //  Create the Stars
  for (int i = 0; i < num_stars; i++) {
    // Create Random Positions
    int half = plane_size/2;
    std::uniform_int_distribution<int> uniform_dist1(-half, half);
    float x = uniform_dist1(el);
    std::uniform_int_distribution<int> uniform_dist2(0, plane_size);
    float z = -1*uniform_dist2(el);

    // Create Stars
    engine::RigidBody * star = new engine::RigidBody(&star_md);
    glm::vec3 p = {x, 0.0f, z};
    star->SetPosition(p);
    star->SetColor(1, 0, 0, 1);
    stars.push_back(star);
  }

  // Create The Floor
  for (int i = 0; i < plane_size; i++) {
    for (int j = 0; j < plane_size; j++) {
      // Get Coordinates
      int x = i - (plane_size/2);
      int z = -1 * j;

      // Create Floor Tile
      engine::RigidBody * tile = new engine::RigidBody(&floor_md);
      glm::vec3 p = {x, -1.5f, z};
      glm::vec3 scale = {1.0f, 0.1f, 1.0f};
      tile->SetPosition(p);
      tile->SetScale(scale);

      // Determine Colors
      if (i%2 == 0) {
        if (j%2 == 0) {
          tile->SetColor(0.2f, 0.2f, 0.2f, 1.0f);
        } else {
          tile->SetColor(0.5f, 0.5f, 0.5f, 1.0f);
        }
      } else {
        if (j%2 == 0) {
          tile->SetColor(0.5f, 0.5f, 0.5f, 1.0f);
        } else {
          tile->SetColor(0.2f, 0.2f, 0.2f, 1.0f);
        }
      }

      // Add Floor Tile
      floors.push_back(tile);
    }
  }

  // Set The Clear Color (Sky Box)
  glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

  // Enable Depth
  glEnable(GL_DEPTH_TEST);

  // Loop until the user closes the window
  while (!glfwWindowShouldClose(window)) {
    // Inputs
    int h_input = glfwGetKey(window, GLFW_KEY_A) -
                  glfwGetKey(window, GLFW_KEY_D);
    int v_input = glfwGetKey(window, GLFW_KEY_W) -
                  glfwGetKey(window, GLFW_KEY_S);
    int p_input = glfwGetKey(window, GLFW_KEY_Q) -
                  glfwGetKey(window, GLFW_KEY_E);

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
      cursor_captured = false;
    } else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT)) {
      cursor_captured = true;
    }

    // Move The Camera
    camera.Move({h_input*movespeed, p_input*movespeed, v_input*movespeed});

    // Set the rendering viewport location and dimensions
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);

    // Create The Camera Frustum
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    camera.MultProjectionMatrix(width, height);

    // Check if we have the cursor
    if (cursor_captured) {
      glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

      // Get the mouse position
      double c_pos_x = 0.0;
      double c_pos_y = 0.0;
      glfwGetCursorPos(window, &c_pos_x, &c_pos_y);
      c_pos_x = static_cast<int>(c_pos_x)%(width*2);
      c_pos_y = engine::clamp(c_pos_y, 0, height);

      // Calculate angles and axies
      camera_y_angle = static_cast<float>(((c_pos_x/width)*view_range) -
                                         (view_range/2));
      camera_x_angle = static_cast<float>(((c_pos_y/height)*view_range) -
                                         (view_range/2));
      glm::vec3 y_axis = {0.0f, 1.0f, 0.0f};
      glm::vec3 x_axis = {1.0f, 0.0f, 0.0f};

      // Turn the camera
      glm::vec3 zero_axis = {0.0f, 0.0f, 1.0f};
      camera.SetOrientation(0.0f, zero_axis);
      camera.Turn(camera_x_angle, x_axis, false);
      camera.Turn(camera_y_angle, y_axis, false);
    } else {
      glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }

    // Clear the color buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Draw the rigid bodies
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
      // Transform The Camera
      camera.MultViewMatrix();
      // Draw The Stars
      for (int i = 0; i < num_stars; i++) {
        glPushMatrix();
          stars[i]->Draw();
        glPopMatrix();
      }
      // Draw The Floor
      for (int i = 0; i < plane_size; i++) {
        for (int j = 0; j < plane_size; j++) {
          glPushMatrix();
            floors[(i*plane_size)+j]->Draw();
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
