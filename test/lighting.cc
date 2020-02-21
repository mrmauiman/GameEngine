/*
 * Copyright 2020 Maui Kelley
 */

#include <iostream>

#include "lib/glm/vec3.hpp"
#include "src/engine/model.h"
#include "src/engine/camera.h"
#include "src/engine/rigidbody.h"

int main(int argc, char **argv) {
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
  std::string obj_file_name = "data/snowman.obj";
  if (argc > 1) {
    obj_file_name = argv[1];
  }
  engine::Model star_md(obj_file_name);

  // Initialize necessary variables
  engine::RigidBody star(&star_md);
  engine::Camera camera(90, 0.1, 100, false);

  star.setPosition(glm::vec3(0.0f, 0.0f, -2.0f));

  // Set The Clear Color (Sky Box)
  glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

  // light
  GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
  GLfloat mat_shininess[] = { 50.0 };
  GLfloat light_position[] = { 1.0, 1.0, 1.0, 0.0 };
  glShadeModel(GL_SMOOTH);

  glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular);
  glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess);
  glLightfv(GL_LIGHT0, GL_POSITION, light_position);

  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);

  // Enable Depth
  glEnable(GL_DEPTH_TEST);

  // Loop until the user closes the window
  while (!glfwWindowShouldClose(window)) {
    // Set the rendering viewport location and dimensions
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);

    // Create The Camera Frustum
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    camera.multProjectionMatrix(width, height);

    // Get the mouse position
    double c_pos_x = 0.0;
    double c_pos_y = 0.0;
    glfwGetCursorPos(window, &c_pos_x, &c_pos_y);

    // Clear the color buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Draw the rigid bodies
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
      // Transform The Camera
      camera.multViewMatrix();
      // Draw The Stars
      glPushMatrix();
        star.turn(-1, glm::vec3(0.0, 1.0, 0.0), false);
        star.draw();
        star.setScale(glm::vec3(0.3, 0.3, 0.3));
      glPopMatrix();
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
